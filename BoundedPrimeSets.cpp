#include "BoundedPrimeSets.h"

#include <climits>

BPS::BoundedPrimeSets (uint64_t upperBound)
    : upperBound (upperBound)
{
    // 1550 is the largest prime gap in the range [0,2^64).
    // We require at least one prime larger than 'upperBound' to avoid expensive range checking.
    primePool = *(PrimeSieve (upperBound + 1550).Primes ());
}

BPSI::BoundedPrimeSetIterator (const BPS& parent)
    : parent (parent) {}

std::vector<uint64_t>::const_iterator BPSI::PrimesBegin () const
{
    // The first two "primes" are dummy values for a more efficient increment algorithm.
    return primes.cbegin () + 2;
}

std::vector<uint64_t>::const_iterator BPSI::PrimesEnd () const
{
    return primes.cend ();
}

uint64_t BPSI::N () const
{
    return n;
}

BPSI& BPSI::operator++ ()
{
    // Algorithm: Attempt to append the next prime.
    // If that is too large, attempt to increment the last prime.
    // If that is too large, repeatedly remove it and attempt to increment the new last prime.

    // 'primePool' contains at least one prime larger than 'upperBound',
    // and 'indices.back ()' will never exceed pi(upperBound - 1) - 1,
    // so there is no risk of bad index exceptions.
    uint64_t nextPrime = parent.primePool[indices.back () + 1];

    // Attempt to append the next prime.
    if (n * nextPrime < parent.upperBound)
    {
        indices.emplace_back (indices.back () + 1);
        primes.emplace_back (nextPrime);
        n *= nextPrime;
        return *this;
    }

    uint64_t currentPrime = primes.back ();

    // Attempt to increment the last prime (and end the loop).
    // If that is too large, remove the last prime and repeat.
    while ((n / currentPrime) * nextPrime >= parent.upperBound)
    {
        n /= currentPrime;
        // If 'n' has just returned to 1, 'indices' will become {2^64 - 1},
        // 'primes' will become {2^64 - 1, 2^64 - 1},
        // and 'currentPrime' will become 2^64 - 1.
        // The loop will not terminate (assuming 'parent.upperBound' > 1).
        indices.pop_back ();
        primes.pop_back ();
        currentPrime = primes.back ();
        // 'primePool' contains at least one prime larger than 'upperBound',
        // and 'indices.back ()' will never exceed pi(upperBound - 1) - 1,
        // so there is no risk of bad index exceptions.
        nextPrime = parent.primePool[indices.back () + 1];
    }

    // Increment the last prime.
    // If 'n' has just returned to 1, it will now become 0
    // since 'currentPrime' has just become 2^64 - 1,
    // marking the end iterator.
    indices[indices.size () - 1]++;
    primes[primes.size () - 1] = nextPrime;
    n = (n / currentPrime) * nextPrime;
    return *this;
}

BPSI BPSI::operator++ (int)
{
    BPSI old = *this;
    operator++ ();
    return old;
}

bool operator== (const BPSI& left, const BPSI& right)
{
    return left.n == right.n;
}

BPSI BPS::Begin () const
{
    BPSI begin (*this);
    begin.n = 1;
    // The first index is 2^64 - 1 for all normal iterators and is incremented to 0 upon reaching the end.
    begin.indices = {std::numeric_limits<uint64_t>::max ()};
    // The first two "primes" are dummy values for a more efficient increment algorithm.
    begin.primes = {std::numeric_limits<uint64_t>::max (), std::numeric_limits<uint64_t>::max ()};
    return begin;
}

BPSI BPS::End () const
{
    BPSI end (*this);
    end.n = 0;
    // There is no need to initialize 'indices' or 'primes' as they are not considered in iterator comparison.
    return end;
}
