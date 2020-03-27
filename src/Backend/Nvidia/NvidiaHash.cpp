// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

//////////////////////////////////////
#include "Backend/Nvidia/NvidiaHash.h"
//////////////////////////////////////

#include "Nvidia/Argon2.h"
#include "Config/Config.h"

/* Salt is not altered by nonce. We can initialize it once per job here. */
void NvidiaHash::init(const NvidiaState &state)
{
    m_state = state;
}

HashResult NvidiaHash::hash(const uint32_t startNonce)
{
    m_state.localNonce = startNonce;
    return nvidiaHash(m_state);
}

NvidiaHash::NvidiaHash(
    const uint32_t memoryKB,
    const uint32_t iterations):
    m_memory(memoryKB),
    m_time(iterations)
{
}
