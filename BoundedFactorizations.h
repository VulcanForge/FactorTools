#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "BoundedPrimeSets.h"
#include "PrimePower.h"

using primes_t = std::vector<uint64_t>;
using factorization_t = std::vector<PrimePower<uint64_t, uint32_t>>;

// Iterates through a specified set of prime factorizations.
// The set is constrained by an upper bound on the integer corresponding
// to each factorization, and by a predetermined pool of primes
// from which to construct the factorizations.
class BoundedFactorizationIterator
{
private:
    // The upper bound.
    uint64_t upperBound;

    // The prime pool.
    std::shared_ptr<const primes_t> primePool;

    // The current factorization.
    std::shared_ptr<factorization_t> factorization;

    // The set of primes appearing in the current prime factorization.
    std::unique_ptr<BoundedPrimeSetIterator> bpsi;

    // The integer corresponding to the current factorization.
    uint64_t n;

    // Whether the iterator is in the end state.
    bool isEnd;

public:
    // Constructs a BoundedFactorizationIterator with given upper bound.
    // The prime pool is constructed to be the set of primes less than the upper bound.
    BoundedFactorizationIterator (uint64_t upperBound);

    // Constructs a BoundedFactorizationIterator with given upper bound and prime pool.
    BoundedFactorizationIterator (uint64_t upperBound, std::shared_ptr<const primes_t> primePool);

    // Returns the current factorization.
    std::shared_ptr<const factorization_t> Factorization () const;

    // Returns the integer corresponding to the current factorization.
    uint64_t N () const;

    // Moves the iterator forward one step.
    void operator++ ();

    // Returns whether the iterator is in the end state.
    bool IsEnd () const;

    // Returns the Moebius function of the integer corresponding to the current factorization.
    int32_t MoebiusN () const;
};
