// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Types/HashDevice.h"
#include "Types/JobSubmit.h"
#include "PoolCommunication/PoolCommunication.h"

class HashManager
{
  public:
    HashManager(const std::shared_ptr<PoolCommunication> pool);

    /* Used to increment the number of hashes performed. Should be used along
       with submitValidHash. submitHash will increment the hashes performed
       on your behalf. */
    void incrementHashesPerformed(
        const uint32_t hashesPerformed,
        const std::string &deviceName);

    /* Call this to submit a hash to the pool that is above the diff. */
    void submitValidHash(const JobSubmit &jobSubmit);

    /* Call this to submit a hash to the pool. We will check the diff. */
    void submitHash(const JobSubmit &jobSubmit);

    /* Call this when a share got accepted by the pool. */
    void shareAccepted();

    /* Print the current stats */
    void printStats();

    /* Pause hashrate monitoring */
    void pause();

    /* Start hashrate monitoring */
    void start();

    /* Reset accepted/submitted count, for example when changing pools */
    void resetShareCount();
    
  private:
    /* Total number of hashes we have performed */
    std::atomic<uint64_t> m_totalHashes = 0;

    /* Total number of hashes we have submitted (that are above the difficulty) */
    std::atomic<uint64_t> m_submittedHashes = 0;

    /* Total number of submitted hashes that were accepted by the pool */
    std::atomic<uint64_t> m_acceptedHashes = 0;

    const std::shared_ptr<PoolCommunication> m_pool;

    /* The effective time we started mining. When we start/stop, we alter this
       based on when we stopped. So, taking now() - effectiveStartTime should
       give the correct duration we have been mining on this manager for. */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_effectiveStartTime;

    /* Time point when we paused. Used to alter the effectiveStartTime when we
       resume again. */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_pauseTime;

    bool m_paused = false;

    std::unordered_map<std::string, HashDevice> m_hashProducers;
};
