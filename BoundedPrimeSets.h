#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "PrimeSieve.h"

// Represents the set of sets of primes whose product is less than a given upper bound.
class BoundedPrimeSets
{
private:
    // The upper bound.
    uint64_t upperBound;

    // The primes less than 'upperBound'.
    std::vector<uint64_t> primePool;

public:
    // Constructs a BoundedPrimeSets object with the given upper bound.
    BoundedPrimeSets (uint64_t upperBound)
        : upperBound (upperBound)
    {
        // 1550 is the largest prime gap in the range [0,2^64).
        // We require at least one prime larger than 'upperBound' to avoid expensive range checking.
        primePool = *(PrimeSieve (upperBound + 1550).Primes ());
    }

    // Represents an iterator over the sets of primes represented by a given BoundedPrimeSets object.
    class BoundedPrimeSetIterator
    {
    private:
        // The BoundedPrimeSets object.
        const BoundedPrimeSets& parent;

        // The product of the current set of primes.
        uint64_t n;

        // The indices in 'parent.primePool' of the primes in the current set.
        std::vector<size_t> indices;

        // The current set of primes.
        std::vector<uint64_t> primes;

        // Constructs a BoundedPrimeSetIterator object with the given parent.
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeSets.
        BoundedPrimeSetIterator (const BoundedPrimeSets& parent)
            : parent (parent) {}

    public:
        // Returns an iterator to the beginning of the current set of primes.
        std::vector<uint64_t>::const_iterator PrimesBegin () const
        {
            // The first two "primes" are dummy values for a more efficient increment algorithm.
            return primes.cbegin () + 2;
        }

        // Returns an iterator to the end of the current set of primes.
        std::vector<uint64_t>::const_iterator PrimesEnd () const
        {
            return primes.cend ();
        }

        // Returns the product of the current set of primes.
        uint64_t N () const
        {
            return n;
        }

        BoundedPrimeSetIterator& operator++ ()
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

        BoundedPrimeSetIterator operator++ (int)
        {
            BoundedPrimeSetIterator old = *this;
            operator++ ();
            return old;
        }

        // Returns the value of the Moebius function at 'n'.
        int16_t MoebiusN () const
        {
            // Efficient (-1)^n algorithm.
            return (-(primes.size () & 1)) | 1;
        }

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedPrimeSetIterator& left, const BoundedPrimeSetIterator& right)
        {
            return left.n == right.n;
        }

        friend class BoundedPrimeSets;

        friend class BoundedPrimeSetProducts;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedPrimeSetIterator Begin () const
    {
        BoundedPrimeSetIterator begin (*this);
        begin.n = 1;
        // The first index is 2^64 - 1 for all normal iterators and is incremented to 0 upon reaching the end.
        begin.indices = {UINT64_MAX};
        // The first two "primes" are dummy values for a more efficient increment algorithm.
        begin.primes = {UINT64_MAX, UINT64_MAX};
        return begin;
    }

    // Returns an iterator to the end of the prescribed set.
    BoundedPrimeSetIterator End () const
    {
        BoundedPrimeSetIterator end (*this);
        end.n = 0;
        // There is no need to initialize 'indices' or 'primes' as they are not considered in iterator comparison.
        return end;
    }

    friend class BoundedFactorizations;

    friend class BoundedPrimeSetProducts;
};

using BPS = BoundedPrimeSets;

using BPSI = BoundedPrimeSets::BoundedPrimeSetIterator;
