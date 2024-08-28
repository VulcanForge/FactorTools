#pragma once

#include <concepts>
#include <cstdint>

#include "Exponent.h"

// A prime power.
template<std::unsigned_integral TPrime, std::unsigned_integral TPower>
struct PrimePower
{
    // The prime.
    TPrime prime;

    // The power.
    TPower power;

    // Constructs a PrimePower.
    PrimePower (TPrime prime, TPower power)
        : prime (prime), power (power) {}

    // Returns the value of the prime power.
    std::uint64_t N () const
    {
        return Pow (std::uint64_t (prime), power);
    }
};
