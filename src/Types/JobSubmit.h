// Copyright (c) 2019, Zpalmtree
//
// Please see the included LICENSE file for more information.

#pragma once

#include <string>
#include <vector>

struct JobSubmit
{
    /* The actual hash we made */
    const uint8_t *hash;

    /* Identifier for this job for the pool */
    std::string jobID;

    /* The nonce we used to produce this hash */
    uint32_t nonce;

    /* The target we have to beat */
    uint64_t target;

    /* An identifier for who produced this hash, for example 'CPU' or 'GTX 1070' */
    std::string hardwareIdentifier;
};
