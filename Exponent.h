#pragma once

#include <cstdint>

// Exactly computes 'base' to the power 'exponent' if the result fits in a 'uint64_t'.
// Out of range arguments result in undefined behaviour.
uint64_t Pow (uint64_t base, uint64_t exponent);

// Computes 'base' to the power 'exponent' using a binary exponentiation algorithm if the result fits in a 'double'.
// Out of range arguments result in undefined behaviour.
double Pow (double base, uint64_t exponent);
