#include "BoundedFactorizations.h"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

#include "BoundedPrimeSets.h"
#include "Exponent.h"
#include "PrimeSieve.h"

// Factorizations are ordered first by the set of distinct primes in lex order,
// then by the exponent tuples in lex order.

BoundedFactorizationIterator::BoundedFactorizationIterator (std::uint64_t upperBound)
    : upperBound (upperBound),
    factorization (std::make_shared<factorization_t> ()),
    n (1),
    isEnd (upperBound <= 1)
{
    // The pool will still exist even after 'sieve' has been destroyed.
    PrimeSieve<std::uint64_t> sieve (upperBound);
    primePool = sieve.Primes ();
    bpsi = std::make_unique<BoundedPrimeSetIterator> (upperBound, primePool);
}

BoundedFactorizationIterator::BoundedFactorizationIterator
(
    std::uint64_t upperBound,
    std::shared_ptr<const primes_t> primePool
)
    : upperBound (upperBound),
    primePool (primePool),
    factorization (std::make_shared<factorization_t> ()),
    bpsi (std::make_unique<BoundedPrimeSetIterator> (upperBound, primePool)),
    n (1),
    isEnd (upperBound <= 1) {}

std::shared_ptr<const factorization_t> BoundedFactorizationIterator::Factorization () const
{
    return factorization;
}

std::uint64_t BoundedFactorizationIterator::N () const
{
    return n;
}

void BoundedFactorizationIterator::operator++ ()
{
    // Attempt to increment the exponent of one of the primes in the current prime set,
    // starting at the highest possible index and moving backwards.
    std::size_t toIncrement = factorization->size () - 1;

    while (true)
    {
        if (toIncrement == std::numeric_limits<std::size_t>::max ())
        {
            // All valid exponent tuples for the current set of primes have already been observed.
            // Attempt to step to the next set of primes in lex order.
            ++(*bpsi);

            if (bpsi->IsEnd ())
            {
                // All valid prime sets and all valid exponent tuples for each have already been observed.
                // Enter the end state.
                isEnd = true;
                return;
            }

            // The first exponent tuple in lex order for the new prime set is that consisting of 1's.
            factorization->clear ();

            for (std::uint64_t prime : *(bpsi->Primes ()))
                factorization->emplace_back (prime, 1);

            n = bpsi->N ();
            return;
        }

        // Attempt to increment the exponent for the prime at 'toIncrement'.
        auto& primePower = (*factorization)[toIncrement];
        std::uint64_t nextN = n * primePower.prime;

        if (nextN < upperBound)
        {
            // The current guess for 'toIncrement' is correct.
            ++(primePower.power);
            n = nextN;
            return;
        }

        // The current guess for 'toIncrement' is incorrect.
        // Instead, step up one level in the search tree by resetting the exponent
        // at 'toIncrement' to 1 and decrementing 'toIncrement'.
        n /= Pow (primePower.prime, primePower.power - 1);
        primePower.power = 1;
        --toIncrement;
    }
}

bool BoundedFactorizationIterator::IsEnd () const
{
    return isEnd;
}

std::int32_t BoundedFactorizationIterator::MoebiusN () const
{
    for (auto& primePower : *factorization)
        if (primePower.power > 1)
            return 0;

    // Efficient (-1)^n algorithm.
    return (-(factorization->size () & 1)) | 1;
}
