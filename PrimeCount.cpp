#include <PrimeCount.h>

#include <cmath>
#include <cmath>

uint64_t LegendreCount (uint64_t n)
{
    return n / (log (n) - 1);
}

uint64_t LiCount (uint64_t n)
{
    // Wikipedia assures me that this is correct.
    return std::expint (log (n));
}
