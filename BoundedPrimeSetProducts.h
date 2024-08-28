#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "PrimePower.h"

using primes_t = std::vector<std::uint64_t>;
using factorization_t = std::vector<PrimePower<std::uint64_t, std::uint32_t>>;

// Iterates through a specified set of prime factorizations.
// The set is constrained by a predetermined pool of primes,
// for each of which the factorization must have exponent at least 1,
// and by an upper bound on the integer corresponding to each factorization.
class BoundedPrimeSetProductIterator
{
private:
    // The upper bound.
    std::uint64_t upperBound;

    // The prime pool.
    std::shared_ptr<const primes_t> primePool;

    // The current factorization.
    std::shared_ptr<factorization_t> factorization;

    // The integer corresponding to the current factorization.
    std::uint64_t n;

    // Whether the iterator is in the end state.
    bool isEnd;

public:
    // Constructs a BoundedPrimeSetProductIterator with given upper bound and prime pool.
    BoundedPrimeSetProductIterator (std::uint64_t upperBound, std::shared_ptr<const primes_t> primePool);

    // Returns the current factorization.
    std::shared_ptr<const factorization_t> Factorization () const;

    // Returns the integer corresponding to the current factorization.
    std::uint64_t N () const;

    // Moves the iterator forward one step.
    void operator++ ();

    // Returns whether the iterator is in the end state.
    bool IsEnd () const;

    // Returns the Moebius function of the integer corresponding to the current factorization.
    std::int32_t MoebiusN () const;
};
