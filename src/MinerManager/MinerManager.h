// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

#pragma once

#include <memory>
#include <random>
#include <thread>

#include "Backend/IBackend.h"
#include "MinerManager/HashManager.h"
#include "Miner/GetConfig.h"
#include "PoolCommunication/PoolCommunication.h"
#include "Types/IHashingAlgorithm.h"

class MinerManager
{
  public:
    /* CONSTRUCTOR */
    MinerManager(
        const std::shared_ptr<PoolCommunication> pool,
        const std::shared_ptr<HardwareConfig> hardwareConfig,
        const bool areDevPool);

    /* DESTRUCTOR */
    ~MinerManager();

    /* PUBLIC METHODS */
    void start();

    void stop();

    void printStats();

  private:

    /* PRIVATE METHODS */
    void setNewJob(const Job &job);

    void pauseMining();

    void resumeMining();

    void statPrinter();

    /* PRIVATE VARIABLES */

    /* Should we stop the worker funcs */
    std::atomic<bool> m_shouldStop = false;

    /* Pool connection */
    const std::shared_ptr<PoolCommunication> m_pool;

    /* Handles submitting shares and tracking hashrate statistics */
    HashManager m_hashManager;

    /* Handles creating random nonces */
    std::random_device m_device;

    std::mt19937 m_gen;

    std::uniform_int_distribution<uint32_t> m_distribution {0, std::numeric_limits<uint32_t>::max()};

    /* Thread that periodically prints hashrate, etc */
    std::thread m_statsThread;

    /* CPU, GPU, etc hash backends that we are currently using */
    std::vector<std::shared_ptr<IBackend>> m_enabledBackends;

    const std::shared_ptr<HardwareConfig> m_hardwareConfig;

    /* Current algorithm we're mining with */
    std::string m_currentAlgorithm;

    /* Current pool we're hashing on */
    Pool m_currentPool;
};
