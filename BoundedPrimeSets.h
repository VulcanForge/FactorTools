#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

using primes_t = std::vector<std::uint64_t>;
using indices_t = std::vector<std::size_t>;

// Iterates through a specified set of sets of primes.
// The set is constrained by an upper bound on the product of each prime set,
// and by a predetermined pool of primes of which each prime set must be a subset.
class BoundedPrimeSetIterator
{
private:
    // The upper bound.
    std::uint64_t upperBound;

    // The prime pool.
    std::shared_ptr<const primes_t> primePool;

    // The indices in 'primePool' of the current prime set.
    indices_t indices;

    // The current prime set.
    std::shared_ptr<primes_t> primes;

    // The product of the current prime set.
    std::uint64_t n;

    // Whether the iterator is in the end state.
    bool isEnd;

public:
    // Constructs a BoundedPrimeSetIterator with the given upper bound.
    // The prime pool is constructed to be the set of primes less than the upper bound.
    BoundedPrimeSetIterator (std::uint64_t upperBound);

    // Constructs a BoundedPrimeSetIterator with the given upper bound and prime pool.
    BoundedPrimeSetIterator (std::uint64_t upperBound, std::shared_ptr<const primes_t> primePool);

    // Returns the current prime set.
    std::shared_ptr<const primes_t> Primes () const;

    // Returns the product of the current prime set.
    std::uint64_t N () const;

    // Moves the iterator forward one step.
    void operator++ ();

    // Returns whether the iterator is in the end state.
    bool IsEnd () const;

    // Returns the Moebius function of the product of the current prime set.
    std::int32_t MoebiusN () const;
};
