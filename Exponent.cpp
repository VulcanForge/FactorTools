#include <Exponent.h>

uint64_t Pow (uint64_t base, uint64_t exponent)
{
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
