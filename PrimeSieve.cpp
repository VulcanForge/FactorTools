#include "PrimeSieve.h"

#include <algorithm>
#include <iostream>
#include <iterator>

PrimeSieve::PrimeSieve (size_t limit, bool verbose)
    : limit (limit), sieve (limit, true), primes (std::make_shared<std::vector<uint64_t>> ())
{
    sieve.Reset (0);
    sieve.Reset (1);
    uint64_t prime = 2;
    primes->emplace_back (2);

    if (verbose)
        while (prime * prime < limit)
        {
            std::clog << "Striking out multiples of " << prime << "\n";

            for (uint64_t i = prime * prime; i < limit; i += prime)
                sieve.Reset (size_t (i));

            for (uint64_t i = prime + 1; ; i++)
                if (sieve.Get (size_t (i)))
                {
                    prime = i;
                    primes->emplace_back (i);
                    break;
                }
        }
    else
        while (prime * prime < limit)
        {
            for (uint64_t i = prime * prime; i < limit; i += prime)
                sieve.Reset (size_t (i));

            for (uint64_t i = prime + 1; ; i++)
                if (sieve.Get (size_t (i)))
                {
                    prime = i;
                    primes->emplace_back (i);
                    break;
                }
        }

    for (uint64_t i = prime + 1; i < limit; i++)
        if (sieve.Get (size_t (i)))
            primes->emplace_back (i);
}

size_t PrimeSieve::Limit () const
{
    return limit;
}

std::shared_ptr<const std::vector<uint64_t>> PrimeSieve::List () const
{
    return primes;
}

std::vector<uint64_t>::const_iterator PrimeSieve::ListBegin () const
{
    return primes->cbegin ();
}

std::vector<uint64_t>::const_iterator PrimeSieve::ListEnd () const
{
    return primes->cend ();
}

size_t PrimeSieve::Count () const
{
    return primes->size ();
}

size_t PrimeSieve::PrimePi (size_t n) const
{
    auto nextPrime = std::upper_bound (primes->cbegin (), primes->cend (), n);
    return std::distance (primes->cbegin (), nextPrime);
}

bool PrimeSieve::IsPrime (size_t n) const
{
    return sieve.Get (n);
}
