#pragma once

#include <cstdint>

// Returns Legendre's approximation for the number of primes in [0, 'n'].
std::uint64_t LegendreCount (std::uint64_t n);

// Returns the logarithmic integral approximation for the number of primes in [0, 'n'].
std::uint64_t LiCount (std::uint64_t n);
