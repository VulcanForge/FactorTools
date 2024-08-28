#include "PrimeCount.h"

#include <cmath>
#include <cstdint>

std::uint64_t LegendreCount (std::uint64_t n)
{
    return n / (std::log (n) - 1);
}

std::uint64_t LiCount (std::uint64_t n)
{
    // Wikipedia assures me that this is correct.
    return std::expint (std::log (n));
}
