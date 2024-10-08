#include "BoundedPrimeSetProducts.h"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

#include "Exponent.h"

// Factorizations are ordered by the exponent tuples in lex order.

BoundedPrimeSetProductIterator::BoundedPrimeSetProductIterator
(
    std::uint64_t upperBound,
    std::shared_ptr<const primes_t> primePool
)
    : upperBound (upperBound),
    primePool (primePool),
    factorization (std::make_shared<factorization_t> ())
{
    // The first exponent in lex order is that consisting of 1's.
    n = 1;

    for (std::uint64_t prime : *primePool)
    {
        factorization->emplace_back (prime, 1);
        n *= prime;
    }

    isEnd = (n >= upperBound);
}

std::shared_ptr<const factorization_t> BoundedPrimeSetProductIterator::Factorization () const
{
    return factorization;
}

std::uint64_t BoundedPrimeSetProductIterator::N () const
{
    return n;
}

void BoundedPrimeSetProductIterator::operator++ ()
{
    // Attempt to increment one of the exponents, starting at the highest possible index and moving backwards.
    std::size_t toIncrement = factorization->size () - 1;

    while (true)
    {
        // All valid exponent tuples have already been observed.
        // Enter the end state.
        if (toIncrement == std::numeric_limits<std::size_t>::max ())
        {
            isEnd = true;
            return;
        }

        // Attempt to increment the exponent for the prime at 'toIncrement'.
        auto& primePower = (*factorization)[toIncrement];
        std::uint64_t nextN = n * primePower.prime;

        if (nextN < upperBound)
        {
            // The current guess for 'toIncrement' is correct.
            ++primePower.power;
            n = nextN;
            return;
        }

        // The current guess for 'toIncrement' is incorrect.
        // Step up one level in the search tree by resetting the exponent
        // at 'toIncrement' to 1 and decrementing 'toIncrement'.
        n /= IntegerPow (primePower.prime, primePower.power - 1);
        primePower.power = 1;
        --toIncrement;
    }
}

bool BoundedPrimeSetProductIterator::IsEnd () const
{
    return isEnd;
}

std::int32_t BoundedPrimeSetProductIterator::MoebiusN () const
{
    for (auto& primePower : *factorization)
        if (primePower.power > 1)
            return 0;

    // Efficient (-1)^n algorithm.
    return (-(factorization->size () & 1)) | 1;
}
