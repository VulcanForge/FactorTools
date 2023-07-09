#pragma once

#include <cstdint>

// A prime power.
struct PrimePower
{
    // The prime.
    uint64_t prime;

    // The power.
    uint64_t power;

    // Constructs a PrimePower.
    PrimePower (uint64_t prime, uint64_t power);
};
