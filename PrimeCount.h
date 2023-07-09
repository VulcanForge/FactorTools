#pragma once

#include <cstdint>

// Returns Legendre's approximation for the number of primes in [0, 'n'].
uint64_t LegendreCount (uint64_t n);

// Returns the logarithmic integral approximation for the number of primes in [0, 'n'].
uint64_t LiCount (uint64_t n);
