// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

#pragma once

#include <atomic>

struct HashDevice
{
    std::atomic<uint64_t> totalHashes;
};
