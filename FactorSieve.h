#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

#include <PrimePower.h>

// A lookup table of the least prime factor for each positive integer.
template<std::unsigned_integral T = uint64_t>
class FactorSieve
{
private:
    // The underlying lookup table.
    std::vector<T> sieve;

    // The exclusive upper bound on the lookup table.
    T limit;

public:
    // Constructs a FactorSieve over [0, 'limit') and optionally outputs progress to 'clog'.
    FactorSieve (T limit, bool verbose = false)
        : limit (limit), sieve (limit, std::numeric_limits<T>::max ())
    {
        std::iota (sieve.begin (), sieve.end (), T (0));
        T prime = 2;

        if (verbose)
            while (prime * prime < limit)
            {
                std::clog << "Marking multiples of " << prime << "\n";

                for (T i = prime * prime; i < limit; i += prime)
                    sieve[i] = std::min (sieve[i], prime);

                for (T i = prime + 1; ; i++)
                    if (sieve[i] == i)
                    {
                        prime = i;
                        break;
                    }
            }
        else
            while (prime * prime < limit)
            {
                for (T i = prime * prime; i < limit; i += prime)
                    sieve[i] = std::min (sieve[i], prime);

                for (T i = prime + 1; ; i++)
                    if (sieve[i] == i)
                    {
                        prime = i;
                        break;
                    }
            }
    }

    // Returns the least prime factor of 'n', if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    T LeastPrimeFactor (T n) const
    {
        return sieve[n];
    }

    // Returns the list of prime factors of 'n', if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    std::vector<PrimePower<T, uint16_t>> PrimeFactors (T n) const
    {
        std::vector<PrimePower<T, uint16_t>> primeFactors;
        T prime = sieve[n];
        primeFactors.emplace_back (prime, 1);
        n /= prime;

        while (n != 1)
        {
            prime = sieve[n];

            if (prime == primeFactors[primeFactors.size () - 1].prime)
                primeFactors[primeFactors.size () - 1].power++;
            else
                primeFactors.emplace_back (prime, 1);

            n /= prime;
        }

        return primeFactors;
    }

    // Returns the list of factors of 'n', if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    std::vector<T> Factors (T n) const
    {
        std::vector<T> factors;
        auto primeFactors = PrimeFactors (n);
        size_t addressSize = primeFactors.size ();
        std::vector<size_t> address (addressSize, 0);
        size_t numberFactors = 1;

        for (size_t i = 0; i < addressSize; i++)
            numberFactors *= (primeFactors[i].power + 1);

        for (size_t i = 0; i < numberFactors; i++)
        {
            T factor = 1;

            for (size_t j = 0; j < addressSize; j++)
                factor *= Pow (uint64_t (primeFactors[j].prime), address[j]);

            factors.emplace_back (factor);

            for (size_t j = 0; j < addressSize; j++)
            {
                address[j] = (address[j] + 1) % (primeFactors[j].power + 1);

                if (address[j] > 0)
                    break;
            }
        }

        std::sort (factors.begin (), factors.end ());
        return factors;
    }
};
