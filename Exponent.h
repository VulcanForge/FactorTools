#pragma once

#include <cstdint>

// Exactly computes 'base' to the power 'exponent' if the result fits in a 'std::uint64_t'.
// Out of range arguments result in undefined behaviour.
std::uint64_t IntegerPow (std::uint64_t base, std::uint64_t exponent);

// Computes 'base' to the power 'exponent' using a binary exponentiation algorithm if the result fits in a 'double'.
// Out of range arguments result in undefined behaviour.
double DoublePow (double base, std::uint64_t exponent);
