#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <span>
#include <vector>

#include <BitArray.h>

// An Eratosthenes prime sieve.
template<std::unsigned_integral T = uint64_t>
class PrimeSieve
{
private:
    // The underlying bit array.
    BitArray sieve;

    // The exclusive upper bound on the numbers sieved.
    T limit;

    // The prime numbers in [0, 'limit').
    std::vector<T> primes;

public:
    // Constructs a PrimeSieve over [0, 'limit') and optionally outputs progress to 'clog'.
    PrimeSieve (T limit, bool verbose = false)
        : limit (limit), sieve (limit, true)
    {
        sieve.Reset (0);
        sieve.Reset (1);
        T prime = 2;
        primes.emplace_back (2);

        if (verbose)
            while (prime * prime < limit)
            {
                std::clog << "Striking out multiples of " << prime << "\n";

                for (T i = prime * prime; i < limit; i += prime)
                    sieve.Reset (i);

                for (T i = prime + 1; ; i++)
                    if (sieve.Get (i))
                    {
                        prime = i;
                        primes.emplace_back (i);
                        break;
                    }
            }
        else
            while (prime * prime < limit)
            {
                for (T i = prime * prime; i < limit; i += prime)
                    sieve.Reset (i);

                for (T i = prime + 1; ; i++)
                    if (sieve.Get (i))
                    {
                        prime = i;
                        primes.emplace_back (i);
                        break;
                    }
            }

        for (T i = prime + 1; i < limit; i++)
            if (sieve.Get (i))
                primes.emplace_back (i);
    }

    // Returns the list of primes in [0, 'limit').
    std::span<const T> Primes () const
    {
        return std::span (primes.cbegin (), primes.cend ());
    }

    // Returns the number of primes in [0, 'limit').
    size_t Count () const
    {
        return primes.size ();
    }

    // Returns the number of primes in [0, 'n'], if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    size_t PrimePi (T n) const
    {
        auto nextPrime = std::upper_bound (primes.cbegin (), primes.cend (), n);
        return std::distance (primes.cbegin (), nextPrime);
    }

    // Returns whether 'n' is prime, if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    bool IsPrime (T n) const
    {
        return sieve.Get (n);
    }
};
