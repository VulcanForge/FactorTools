#include "BoundedPrimeFixedSizeSets.h"

#include "PrimeSieve.h"

// Sets are ordered in lex order, and each set is represented in increasing order.

BoundedPrimeFixedSizeSetIterator::BoundedPrimeFixedSizeSetIterator (uint64_t upperBound, uint32_t setSize)
    : upperBound (upperBound),
    setSize (setSize)
{
    // The pool will still exist even after 'sieve' has been destroyed.
    PrimeSieve<uint64_t> sieve (upperBound);
    primePool = sieve.Primes ();
    primes = std::make_shared<primes_t> ();

    // The first subset of 'primePool' of size 'setSize' in lex order
    // is the set consisting of the smallest 'setSize' primes in 'primePool'.
    n = 1;

    for (size_t i = 0; i < setSize; ++i)
    {
        indices.emplace_back (i);
        primes->emplace_back ((*primePool)[i]);
        n *= (*primePool)[i];
    }

    isEnd = (n >= upperBound);
}

BoundedPrimeFixedSizeSetIterator::BoundedPrimeFixedSizeSetIterator
(
    uint64_t upperBound,
    uint32_t setSize,
    std::shared_ptr<const primes_t> primePool
)
    : upperBound (upperBound),
    setSize (setSize),
    primePool (primePool)
{
    // The first subset of 'primePool' of size 'setSize' in lex order
    // is the set consisting of the smallest 'setSize' primes in 'primePool'.
    n = 1;

    for (size_t i = 0; i < setSize; ++i)
    {
        indices.emplace_back (i);
        primes->emplace_back ((*primePool)[i]);
        n *= (*primePool)[i];
    }

    isEnd = (n >= upperBound);
}

std::shared_ptr<const primes_t> BoundedPrimeFixedSizeSetIterator::Primes () const
{
    return primes;
}

uint64_t BoundedPrimeFixedSizeSetIterator::N () const
{
    return n;
}

void BoundedPrimeFixedSizeSetIterator::operator++ ()
{
    // Attempt to replace one of the primes in 'primes' with its successor in 'primePool',
    // starting at the highest possible index in 'primes' and moving backwards,
    // and updating the subsequent primes in 'primes' as necessary
    // to preserve the increasing order and lex order properties.
    size_t toIncrement = indices.size () - 1;

    while (true)
    {
        // If the current guess for 'toIncrement' is correct, 'newLastIndex'
        // contains the index in 'primePool' of the last prime in the correct new value of 'primes'
        size_t newLastIndex = indices[toIncrement] + indices.size () - toIncrement;

        if (newLastIndex < primePool->size ())
        {
            // 'primePool' has enough primes to accommodate the current guess for 'toIncrement'.
            // If the current guess for 'toIncrement' is correct, the correct tail
            // of 'primes' starting at 'toIncrement' will be the subsequence of
            // 'primePool' starting at 'indices[toIncrement] + 1' and ending at 'indices[newLastIndex]'
            // This can be deduced by considering the increasing order and lex order properties.
            for (size_t i = toIncrement, j = indices[toIncrement] + 1; i < indices.size (); ++i, ++j)
            {
                indices[i] = j;
                (*primes)[i] = (*primePool)[j];
            }

            n = 1;

            for (uint64_t prime : *primes)
                n *= prime;

            if (n < upperBound)
                // The current guess for 'toIncrement' is correct, and
                // the current state of 'primes' and 'indices' is a valid state for the iterator.
                return;
        }

        // The current guess for 'toIncrement' is incorrect.
        // Step up one level in the search tree by decrementing 'toIncrement'.
        --toIncrement;

        if (toIncrement == SIZE_MAX)
        {
            // All valid prime sets have already been observed.
            // Enter the end state.
            isEnd = true;
            return;
        }
    }
}

bool BoundedPrimeFixedSizeSetIterator::IsEnd () const
{
    return isEnd;
}

int32_t BoundedPrimeFixedSizeSetIterator::MoebiusN () const
{
    // Efficient (-1)^n algorithm.
    return (-(setSize & 1)) | 1;
}
