#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "BitArray.h"

// An Eratosthenes prime sieve.
template<typename T = uint64_t>
class PrimeSieve
{
private:
    // The underlying bit array.
    BitArray sieve;

    // The exclusive upper bound on the numbers sieved.
    T limit;

    // The prime numbers in [0, 'limit').
    std::shared_ptr<std::vector<T>> primes;

public:
    // Constructs a PrimeSieve over [0, limit) and optionally outputs progress to 'clog'.
    PrimeSieve (T limit, bool verbose = false)
        : limit (limit), sieve (limit, true), primes (std::make_shared<std::vector<T>> ())
    {
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

                for (T i = prime + 1; ; i++)
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

                for (T i = prime + 1; ; i++)
                    if (sieve.Get (i))
                    {
                        prime = i;
                        primes->emplace_back (i);
                        break;
                    }
            }

        for (T i = prime + 1; i < limit; i++)
            if (sieve.Get (i))
                primes->emplace_back (i);
    }

    // Returns the exclusive upper bound on the numbers sieved.
    T Limit () const
    {
        return limit;
    }

    // Returns the list of primes in [0, 'limit').
    std::shared_ptr<const std::vector<T>> Primes () const
    {
        return primes;
    }

    // Returns an iterator to the beginning of the primes in [0, 'limit').
    std::vector<T>::const_iterator PrimesBegin () const
    {
        return primes->cbegin ();
    }

    // Returns an iterator to the end of the primes in [0, 'limit').
    std::vector<T>::const_iterator PrimesEnd () const
    {
        return primes->cend ();
    }

    // Returns the number of primes in [0, 'limit').
    size_t Count () const
    {
        return primes->size ();
    }

    // Returns the number of primes in [0, 'n'], if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    size_t PrimePi (T n) const
    {
        auto nextPrime = std::upper_bound (primes->cbegin (), primes->cend (), n);
        return std::distance (primes->cbegin (), nextPrime);
    }

    // Returns whether 'n' is prime, if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    bool IsPrime (T n) const
    {
        return sieve.Get (n);
    }

    // BEGIN STL-COMPATIBILITY FUNCTIONS

    // Indexes 'primes'.
    const T& operator[] (size_t index) const
    {
        return (*primes)[index];
    }

    // Duplicates of 'PrimesBegin' and 'PrimesEnd' for for-each loops.

    std::vector<T>::const_iterator begin () const
    {
        return primes->cbegin ();
    }

    std::vector<T>::const_iterator end () const
    {
        return primes->cend ();
    }
};
