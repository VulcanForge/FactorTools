#include "PrimeSieve.h"

#include <iostream>

PrimeSieve::PrimeSieve (size_t limit)
    : PrimeSieve (limit, false) {}

PrimeSieve::PrimeSieve (size_t limit, bool verbose)
    : limit (limit), sieve (limit, true)
{
    sieve.Set (0, false);
    sieve.Set (1, false);
    uint64_t prime = 2;
    primes.emplace_back (2);

    if (verbose)
        while (prime * prime < limit)
        {
            std::clog << "Striking out multiples of " << prime << "\n";

            for (uint64_t i = prime * prime; i < limit; i += prime)
                sieve.Set (size_t (i), false);

            for (uint64_t i = prime + 1; ; i++)
                if (sieve.Get (size_t (i)))
                {
                    prime = i;
                    primes.emplace_back (i);
                    break;
                }
        }
    else
        while (prime * prime < limit)
        {
            for (uint64_t i = prime * prime; i < limit; i += prime)
                sieve.Set (size_t (i), false);

            for (uint64_t i = prime + 1; ; i++)
                if (sieve.Get (size_t (i)))
                {
                    prime = i;
                    primes.emplace_back (i);
                    break;
                }
        }

    for (uint64_t i = prime + 1; i < limit; i++)
        if (sieve.Get (size_t (i)))
            primes.emplace_back (i);
}

size_t PrimeSieve::Limit () const
{
    return limit;
}

const std::vector<uint64_t>& PrimeSieve::List () const
{
    return primes;
}

std::vector<uint64_t>::const_iterator PrimeSieve::ListBegin () const
{
    return primes.cbegin ();
}

std::vector<uint64_t>::const_iterator PrimeSieve::ListEnd () const
{
    return primes.cend ();
}

size_t PrimeSieve::Count () const
{
    return primes.size ();
}

bool PrimeSieve::IsPrime (size_t n) const
{
    return sieve.Get (n);
}
