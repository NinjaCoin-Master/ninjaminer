// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

#pragma once

#include "Argon2/Argon2.h"
#include "Types/PoolMessage.h"
#include "Miner/GetConfig.h"
#include "Nvidia/Argon2.h"

class NvidiaHash
{
  public:
    NvidiaHash(
        const uint32_t memoryKB,
        const uint32_t iterations);

    void init(const NvidiaState &state);

    uint32_t getMemory() const { return m_memory; };
    uint32_t getIterations() const { return m_time; };

    HashResult hash(const uint32_t startNonce);

  private:

    const uint32_t m_memory;
    const uint32_t m_time;

    NvidiaState m_state;
};
