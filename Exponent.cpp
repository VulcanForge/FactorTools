#include "Exponent.h"

#include <cstdint>

std::uint64_t Pow (std::uint64_t base, std::uint64_t exponent)
{
    // Standard binary exponential algorithm.
    std::uint64_t power = 1;

    while (exponent > 0)
    {
        if (exponent & 1)
            power *= base;

        exponent /= 2;
        base *= base;
    }

    return power;
}

double Pow (double base, std::uint64_t exponent)
{
    // Standard binary exponentiation algorithm.
    double power = 1;

    while (exponent > 0)
    {
        if (exponent & 1)
            power *= base;

        exponent /= 2;
        base *= base;
    }

    return power;
}
