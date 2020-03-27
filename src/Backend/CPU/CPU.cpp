// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

////////////////////////////
#include "Backend/CPU/CPU.h"
////////////////////////////

#include <iostream>

#include "Types/JobSubmit.h"

CPU::CPU(
    const std::shared_ptr<HardwareConfig> &hardwareConfig,
    const std::function<void(const JobSubmit &jobSubmit)> &submitHashCallback):
    m_hardwareConfig(hardwareConfig),
    m_submitHash(submitHashCallback)
{
}

void CPU::start(const Job &job, const uint32_t initialNonce)
{
    if (!m_threads.empty())
    {
        stop();
    }

    m_shouldStop = false;

    m_nonce = initialNonce;

    m_currentJob = job;

    /* Indicate that there's no new jobs available to other threads */
    m_newJobAvailable = std::vector<bool>(m_hardwareConfig->cpu.threadCount, false);

    for (uint32_t i = 0; i < m_hardwareConfig->cpu.threadCount; i++)
    {
        m_threads.push_back(std::thread(&CPU::hash, this, i));
    }
}

void CPU::stop()
{
    m_shouldStop = true;

    for (int i = 0; i < m_newJobAvailable.size(); i++)
    {
        m_newJobAvailable[i] = true;
    }

    /* Wait for all the threads to stop */
    for (auto &thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    /* Empty the threads vector for later re-creation */
    m_threads.clear();
}

void CPU::setNewJob(const Job &job, const uint32_t initialNonce)
{
    /* Set new nonce */
    m_nonce = initialNonce;

    /* Update stored job */
    m_currentJob = job;

    /* Indicate to each thread that there's a new job */
    for (int i = 0; i < m_newJobAvailable.size(); i++)
    {
        m_newJobAvailable[i] = true;
    }
}

std::vector<PerformanceStats> CPU::getPerformanceStats()
{
    return {};
}

void CPU::hash(const uint32_t threadNumber)
{
    std::string currentAlgorithm;
    NonceInfo nonceInfo;

    while (!m_shouldStop)
    {
        uint32_t localNonce = m_nonce;

        Job job = m_currentJob;

        const bool isNiceHash = job.isNiceHash;

        auto algorithm = ArgonVariant::getCPUMiningAlgorithm(m_currentJob.algorithm);

        if (job.algorithm != currentAlgorithm)
        {
            nonceInfo = m_hardwareConfig->getNonceOffsetInfo("cpu");
            currentAlgorithm = job.algorithm;
        }

        /* Let the algorithm perform any necessary initialization */
        algorithm->init(m_currentJob.rawBlob);
        algorithm->reinit(m_currentJob.rawBlob);

        int i = 0;

        while (!m_newJobAvailable[threadNumber])
        {
            const uint32_t ourNonce = localNonce + (i * nonceInfo.noncesPerRound) + threadNumber;

            /* If nicehash mode is enabled, we are only allowed to alter 3 bytes
               in the nonce, instead of four. The first byte is reserved for nicehash
               to do with as they like.
               To achieve this, we wipe the top byte (localNonce & 0x00FFFFFF) of
               local nonce. We then wipe the bottom 3 bytes of job.nonce
               (*job.nonce() & 0xFF000000). Finally, we AND them together, so the
               top byte of the nonce is reserved for nicehash.
               See further https://github.com/nicehash/Specifications/blob/master/NiceHash_CryptoNight_modification_v1.0.txt
               Note that the above specification indicates that the final byte of
               the nonce is reserved, but in fact it is the first byte that is 
               reserved. */
            if (isNiceHash)
            {
                *job.nonce() = (ourNonce & 0x00FFFFFF) | (*job.nonce() & 0xFF000000);
            }
            else
            {
                *job.nonce() = ourNonce;
            }

            const auto hash = algorithm->hash(job.rawBlob);

            m_submitHash({ hash.data(), job.jobID, *job.nonce(), job.target, "CPU" });

            i++;

            /* If not all hardware has checked in with the new job, keep attempting
             * to fetch it to ensure we're not doing duplicate work. */
            if (!nonceInfo.allHardwareInitialized)
            {
                nonceInfo = m_hardwareConfig->getNonceOffsetInfo("cpu");
            }
        }

        /* Switch to new job. */
        m_newJobAvailable[threadNumber] = false;
    }
}
