// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

//////////////////////////////////////
#include "MinerManager/MinerManager.h"
//////////////////////////////////////

#include <iostream>
#include <sstream>
#include <iomanip>

#include "Backend/CPU/CPU.h"
#include "Types/JobSubmit.h"
#include "Utilities/ColouredMsg.h"
#include "Utilities/Utilities.h"

#if defined(NVIDIA_ENABLED)
#include "Backend/Nvidia/Nvidia.h"
#endif

MinerManager::MinerManager(
    const std::shared_ptr<PoolCommunication> pool,
    const std::shared_ptr<HardwareConfig> hardwareConfig,
    const bool areDevPool):
    m_pool(pool),
    m_hashManager(pool),
    m_hardwareConfig(hardwareConfig),
    m_gen(m_device())
{
    const auto submit = [this](const JobSubmit &jobSubmit)
    {
        m_hashManager.submitHash(jobSubmit);
    };

    if (hardwareConfig->cpu.enabled)
    {
        m_enabledBackends.push_back(std::make_shared<CPU>(hardwareConfig, submit));
    }
    else if (!areDevPool)
    {
        std::cout << WarningMsg("CPU mining disabled.") << std::endl;
    }

    const bool allNvidiaGPUsDisabled = std::none_of(
        hardwareConfig->nvidia.devices.begin(),
        hardwareConfig->nvidia.devices.end(),
        [](const auto device)
        {
            return device.enabled;
        }
    );

    #if defined(NVIDIA_ENABLED)
    const auto submitValid = [this](const JobSubmit &jobSubmit)
    {
        m_hashManager.submitValidHash(jobSubmit);
    };

    const auto increment = [this](const uint32_t hashesPerformed, const std::string &deviceName)
    {
        m_hashManager.incrementHashesPerformed(hashesPerformed, deviceName);
    };

    if (!allNvidiaGPUsDisabled)
    {
        m_enabledBackends.push_back(
            std::make_shared<Nvidia>(hardwareConfig, submitValid, increment)
        );
    }
    else if (!areDevPool)
    {
        std::cout << WarningMsg("No Nvidia GPUs available, or all disabled, not starting Nvidia mining") << std::endl;
    }
    #endif
}

MinerManager::~MinerManager()
{
    stop();
}

void MinerManager::setNewJob(const Job &job)
{
    /* Set new nonce */
    const uint32_t nonce = m_distribution(m_gen);

    if (job.algorithm != m_currentAlgorithm)
    {
        m_currentAlgorithm = job.algorithm;

        for (auto &gpu : m_hardwareConfig->nvidia.devices)
        {
            if (gpu.enabled)
            {
                gpu.checkedIn = false;
            }
        }

        for (auto &gpu : m_hardwareConfig->amd.devices)
        {
            if (gpu.enabled)
            {
                gpu.checkedIn = false;
            }
        }
    }

    for (auto &backend : m_enabledBackends)
    {
        backend->setNewJob(job, nonce);
    }

    m_pool->printPool();

    /* Let the user know we got a new job */
    std::cout << WhiteMsg("New job, diff ") << WhiteMsg(job.shareDifficulty) << std::endl;
}

void MinerManager::start()
{
    if (m_statsThread.joinable())
    {
        stop();
    }

    m_shouldStop = false;

    /* Hook up the function to set a new job when it arrives */
    m_pool->onNewJob([this](const Job &job){
        setNewJob(job);
    });

    /* Pass through accepted shares to the hash manager */
    m_pool->onHashAccepted([this](const auto &){
        m_hashManager.shareAccepted();
    });

    /* Start mining when we connect to a pool */
    m_pool->onPoolSwapped([this](const Pool &newPool){

        /* New pool, accepted/submitted count no longer applies */
        if (newPool != m_currentPool) {
            m_hashManager.resetShareCount();
        }

        m_currentPool = newPool;

        resumeMining();
    });

    /* Stop mining when we disconnect */
    m_pool->onPoolDisconnected([this](){
        pauseMining();
    });

    /* Start listening for messages from the pool */
    m_pool->startManaging();
}

void MinerManager::resumeMining()
{
    if (m_statsThread.joinable())
    {
        pauseMining();
    }

    m_shouldStop = false;

    std::cout << WhiteMsg("Resuming mining.") << std::endl;

    const auto job = m_pool->getJob();

    m_pool->printPool();
    std::cout << WhiteMsg("New job, diff ") << WhiteMsg(job.shareDifficulty) << std::endl;

    /* Set initial nonce */
    const uint32_t nonce = m_distribution(m_gen);

    for (auto &backend : m_enabledBackends)
    {
        backend->start(job, nonce);
    }

    /* Launch off the thread to print stats regularly */
    m_statsThread = std::thread(&MinerManager::statPrinter, this);
}

void MinerManager::pauseMining()
{
    std::cout << WhiteMsg("Pausing mining.") << std::endl;

    m_shouldStop = true;

    for (auto &backend : m_enabledBackends)
    {
        backend->stop();
    }

    /* Pause the hashrate calculator */
    m_hashManager.pause();

    if (m_statsThread.joinable())
    {
        m_statsThread.join();
    }
}

void MinerManager::stop()
{
    m_shouldStop = true;

    for (auto &backend : m_enabledBackends)
    {
        backend->stop();
    }

    /* Pause the hashrate calculator */
    m_hashManager.pause();

    /* Wait for the stats thread to stop */
    if (m_statsThread.joinable())
    {
        m_statsThread.join();
    }

    /* Close the socket connection to the pool */
    if (m_pool)
    {
        m_pool->logout();
    }
}

void MinerManager::printStats()
{
    m_hashManager.printStats();
}

void MinerManager::statPrinter()
{
    m_hashManager.start();

    while (!m_shouldStop)
    {
        Utilities::sleepUnlessStopping(std::chrono::seconds(20), m_shouldStop);
        printStats();
    }
}
