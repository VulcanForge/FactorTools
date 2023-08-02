#pragma once

#include <concepts>
#include <cstdint>

// A prime power.
template<std::unsigned_integral TPrime = uint64_t, std::unsigned_integral TPower = uint16_t>
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
