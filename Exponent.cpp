#include <Exponent.h>

uint64_t Pow (uint64_t base, uint64_t exponent)
{
    // Standard binary exponential algorithm.
    uint64_t power = 1;

    while (exponent > 0)
    {
        if (exponent & 1)
            power *= base;

        exponent /= 2;
        base *= base;
    }

    return power;
}

double Pow (double base, uint64_t exponent)
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
