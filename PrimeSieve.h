#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "BitArray.h"

// An Eratosthenes prime sieve.
template<std::unsigned_integral T>
class PrimeSieve
{
private:
    // The underlying bit array.
    BitArray sieve;

    // The exclusive upper bound on the numbers sieved.
    T limit;

    // The primes in [0, 'limit').
    std::shared_ptr<std::vector<T>> primes;

public:
    // Constructs a PrimeSieve over [0, 'limit') and optionally outputs progress to 'clog'.
    PrimeSieve (T limit, bool verbose = false)
        : limit (limit),
        sieve (limit, true),
        primes (std::make_shared<std::vector<T>> ())
    {
        // Standard Eratosthenes sieve algorithm.
        sieve.Reset (0);
        sieve.Reset (1);
        T prime = 2;
        primes->emplace_back (2);

        if (verbose)
            while (prime * prime < limit)
            {
                std::clog << "Striking out multiples of " << prime << "\n";

                for (T i = prime * prime; i < limit; i += prime)
                    sieve.Reset (i);

                for (T i = prime + 1; ; ++i)
                    if (sieve.Get (i))
                    {
                        prime = i;
                        primes->emplace_back (i);
                        break;
                    }
            }
        else
            while (prime * prime < limit)
            {
                for (T i = prime * prime; i < limit; i += prime)
                    sieve.Reset (i);

                for (T i = prime + 1; ; ++i)
                    if (sieve.Get (i))
                    {
                        prime = i;
                        primes->emplace_back (i);
                        break;
                    }
            }

        for (T i = prime + 1; i < limit; ++i)
            if (sieve.Get (i))
                primes->emplace_back (i);
    }

    // Returns the primes in [0, 'limit').
    std::shared_ptr<const std::vector<T>> Primes () const
    {
        return primes;
    }

    // Returns the number of primes in [0, 'limit').
    std::size_t Count () const
    {
        return primes->size ();
    }

    // Returns the number of primes in [0, 'n'], if 'n' is in [0, 'limit').
    // Out of range arguments result in undefined behaviour.
    std::size_t PrimePi (T n) const
    {
        // Find the first prime which exceeds 'n'.
        auto nextPrime = std::upper_bound (primes->cbegin (), primes->cend (), n);
        // Return the index of the prime just discovered.
        return std::distance (primes->cbegin (), nextPrime);
    }

    // Returns whether 'n' is prime, if 'n' is in [0, 'limit').
    // Out of range arguments result in undefined behaviour.
    bool IsPrime (T n) const
    {
        return sieve.Get (n);
    }
};
