#include <PrimeCount.h>

#include <cmath>
#include <numbers>

uint64_t LegendreCount (uint64_t n)
{
    return n / (log (n) - 1);
}

uint64_t LiCount (uint64_t n)
{
    double logn = log (n);
    double li = std::numbers::egamma + log (logn);
    double term;
    double i = 1;
    double powlog = logn;
    double ifactorial = 1;

    do
    {
        term = powlog / (ifactorial * i);
        li += term;
        i++;
        powlog *= logn;
        ifactorial *= i;
    } while (li + term != li);

    return li;
}
