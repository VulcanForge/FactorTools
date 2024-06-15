#include "BoundedPrimeSets.h"

#include "PrimeSieve.h"

// Sets are ordered in lex order, starting at the empty set, and each set is represented in increasing order.

BoundedPrimeSetIterator::BoundedPrimeSetIterator (uint64_t upperBound)
    : upperBound (upperBound),
    primes (std::make_shared<primes_t> ()),
    n (1),
    isEnd (upperBound <= 1)
{
    // The pool will still exist even after 'sieve' has been destroyed.
    PrimeSieve<uint64_t> sieve (upperBound);
    primePool = sieve.Primes ();
}

BoundedPrimeSetIterator::BoundedPrimeSetIterator
(
    uint64_t upperBound,
    std::shared_ptr<const primes_t> primePool
)
    : upperBound (upperBound),
    primePool (primePool),
    primes (std::make_shared<primes_t> ()),
    n (1),
    isEnd (upperBound <= 1) {}

std::shared_ptr<const primes_t> BoundedPrimeSetIterator::Primes () const
{
    return primes;
}

uint64_t BoundedPrimeSetIterator::N () const
{
    return n;
}

void BoundedPrimeSetIterator::operator++ ()
{
    // Try to append to 'primes' the prime in 'primePool' succeeding the last prime in 'primes'.
    if (indices.empty ())
    {
        // The previous prime set was the empty set.
        // The next set in lex order is the singleton containing 'primePool.front ()',
        // assuming that prime is not larger than 'upperBound'.
        if (primePool->front () < upperBound)
        {
            // The singleton containing 'primePool.front ()' is valid.
            indices.emplace_back (0);
            primes->emplace_back (primePool->front ());
            n = primes->front ();
            return;
        }

        // No non-empty subsets of 'primePool' are valid.
        // Enter the end state.
        isEnd = true;
        return;
    }

    if (indices.back () < primePool->size () - 1)
    {
        // There is a prime 'nextPrime' in 'primePool' greater than any prime in 'primes'.
        // First try to append 'nextPrime' to 'primes'.
        uint64_t nextPrime = (*primePool)[indices.back () + 1];
        uint64_t nextN = n * nextPrime;

        if (nextN < upperBound)
        {
            // Appending 'nextPrime' results in a valid set.
            indices.emplace_back (indices.back () + 1);
            primes->emplace_back (nextPrime);
            n = nextN;
            return;
        }

        // Appending 'nextPrime' results in an invalid set.
        // Instead of appending, try replacing the last prime in 'primes' with 'nextPrime'.
        nextN = n / primes->back () * nextPrime;

        if (nextN < upperBound)
        {
            // Replacing the last prime in 'primes' with 'nextPrime' results in a valid set.
            ++indices.back ();
            primes->back () = nextPrime;
            n = nextN;
            return;
        }
    }

    // Repeatedly remove the last prime in 'primes' and replace the new last prime with its successor in 'primePool'.
    while (true)
    {
        // Move up the search tree.
        n /= primes->back ();
        primes->pop_back ();
        indices.pop_back ();

        if (indices.empty ())
        {
            // All valid sets have already been observed.
            // Enter the end state.
            isEnd = true;
            return;
        }

        // Try to replace the last prime with its successor in 'primePool'.
        uint64_t nextPrime = (*primePool)[indices.back () + 1];
        uint64_t nextN = n / primes->back () * nextPrime;

        if (nextN < upperBound)
        {
            // Replacing the last prime in 'primes' with its successor in 'primePool' results in a valid set.
            ++indices.back ();
            primes->back () = nextPrime;
            n = nextN;
            return;
        }
    }
}

bool BoundedPrimeSetIterator::IsEnd () const
{
    return isEnd;
}

int32_t BoundedPrimeSetIterator::MoebiusN () const
{
    // Efficient (-1)^n algorithm.
    return (-(primes->size () & 1)) | 1;
}
