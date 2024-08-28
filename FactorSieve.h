#pragma once

#include <algorithm>
#include <cstddef>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <vector>

#include "Exponent.h"
#include "PrimePower.h"

// A Eratosthenes-type sieve for computing the least prime factor for a range of positive integers.
template<std::unsigned_integral T>
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
        : limit (limit)
    {
        // Fill the sieve with the integers in [0, 'limit').
        for (T i = 0; i < limit; ++i)
            sieve.emplace_back (i);

        T prime = 2;

        if (verbose)
            while (prime * prime < limit)
            {
                std::clog << "Marking multiples of " << prime << "\n";

                // For each 'multiple' of 'prime', if a smaller prime factor of 'multiple' has not been discovered,
                // mark 'prime' as the smallest prime factor of 'multiple'.
                for (T multiple = prime * prime; multiple < limit; multiple += prime)
                    sieve[multiple] = std::min (sieve[multiple], prime);

                // Find the next prime to continue the sieve operation.
                for (T i = prime + 1; ; ++i)
                    if (sieve[i] == i)
                    {
                        prime = i;
                        break;
                    }
            }
        else
            // For each 'multiple' of 'prime', if a smaller prime factor of 'multiple' has not been discovered,
            // mark 'prime' as the smallest prime factor of 'multiple'.
            while (prime * prime < limit)
            {
                for (T multiple = prime * prime; multiple < limit; multiple += prime)
                    sieve[multiple] = std::min (sieve[multiple], prime);

                // Find the next prime to continue the sieve operation.
                for (T i = prime + 1; ; ++i)
                    if (sieve[i] == i)
                    {
                        prime = i;
                        break;
                    }
            }
    }

    // Returns the least prime factor of 'n', if 'n' is in [0, 'limit').
    // Out of range arguments result in undefined behaviour.
    T LeastPrimeFactor (T n) const
    {
        return sieve[n];
    }

    // Returns the prime factorization of 'n', if 'n' is in [0, 'limit').
    // Out of range arguments result in undefined behaviour.
    std::vector<PrimePower<T, std::uint32_t>> PrimeFactors (T n) const
    {
        std::vector<PrimePower<T, std::uint32_t>> primeFactors;

        // Repeatedly divide 'n' by the smallest prime dividing 'n',
        // then look up the new smallest prime dividing the result.
        T prime = sieve[n];
        primeFactors.emplace_back (prime, 1);
        n /= prime;

        while (n != 1)
        {
            prime = sieve[n];

            // Check if the prime has already occurred (divides the original 'n' with multiplicity).
            if (prime == primeFactors[primeFactors.size () - 1].prime)
                ++(primeFactors[primeFactors.size () - 1].power);
            else
                primeFactors.emplace_back (prime, 1);

            n /= prime;
        }

        return primeFactors;
    }

    // Returns the factors of 'n' in increasing order, if 'n' is in [0, 'limit').
    // Out of range arguments result in undefined behaviour.
    std::vector<T> Factors (T n) const
    {
        std::vector<T> factors;
        auto primeFactors = PrimeFactors (n);
        std::size_t addressSize = primeFactors.size ();
        std::vector<std::size_t> address (addressSize, 0);
        std::size_t numberFactors = 1;

        // Standard product form of divisor counting function.
        for (std::size_t i = 0; i < addressSize; ++i)
            numberFactors *= (primeFactors[i].power + 1);

        // Iterate through all possible exponent tuples on the primes dividing 'n'.
        for (std::size_t i = 0; i < numberFactors; ++i)
        {
            T factor = 1;

            for (std::size_t j = 0; j < addressSize; ++j)
                factor *= IntegerPow (std::uint64_t (primeFactors[j].prime), address[j]);

            factors.emplace_back (factor);

            // Move to the next exponent tuple in lex order.
            for (std::size_t j = 0; j < addressSize; ++j)
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
