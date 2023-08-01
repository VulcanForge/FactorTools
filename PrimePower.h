#pragma once

#include <cstdint>

// A prime power.
template<typename TPrime = uint64_t, typename TPower = uint16_t>
struct PrimePower
{
    // The prime.
    TPrime prime;

    // The power.
    TPower power;

    // Constructs a PrimePower.
    PrimePower (TPrime prime, TPower power)
        : prime (prime), power (power) {}
};
