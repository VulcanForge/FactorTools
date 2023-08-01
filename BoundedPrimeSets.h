#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

#include "PrimeSieve.h"

// Represents the set of sets of primes whose product is less than a given upper bound.
template<typename T = uint64_t>
class BoundedPrimeSets
{
private:
    // The upper bound.
    T upperBound;

    // The primes less than 'upperBound'.
    std::vector<T> primePool;

public:
    // Constructs a BoundedPrimeSets object with the given upper bound.
    BoundedPrimeSets (T upperBound)
        : upperBound (upperBound)
    {
        // 1550 is the largest prime gap in the range [0,2^64).
        // We require at least one prime larger than 'upperBound' to avoid expensive range checking.
        primePool = *(PrimeSieve<T> (upperBound + 1550).Primes ());
    }

    // Represents an iterator over the sets of primes represented by a given BoundedPrimeSets object.
    class BoundedPrimeSetIterator
    {
    private:
        // The BoundedPrimeSets object.
        const BoundedPrimeSets& parent;

        // The product of the current set of primes.
        T n;

        // The indices in 'parent.primePool' of the primes in the current set.
        std::vector<size_t> indices;

        // The current set of primes.
        std::vector<T> primes;

        // Constructs a BoundedPrimeSetIterator object with the given parent.
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeSets.
        BoundedPrimeSetIterator (const BoundedPrimeSets& parent)
            : parent (parent) {}

    public:
        // Returns an iterator to the beginning of the current set of primes.
        std::vector<T>::const_iterator PrimesBegin () const
        {
            // The first two "primes" are dummy values for a more efficient increment algorithm.
            return primes.cbegin () + 2;
        }

        // Returns an iterator to the end of the current set of primes.
        std::vector<T>::const_iterator PrimesEnd () const
        {
            return primes.cend ();
        }

        // Returns the product of the current set of primes.
        T N () const
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
            T nextPrime = parent.primePool[indices.back () + 1];

            // Attempt to append the next prime.
            if (n * nextPrime < parent.upperBound)
            {
                indices.emplace_back (indices.back () + 1);
                primes.emplace_back (nextPrime);
                n *= nextPrime;
                return *this;
            }

            T currentPrime = primes.back ();

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

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedPrimeSetIterator& left, const BoundedPrimeSetIterator& right)
        {
            return left.n == right.n;
        }

        friend class BoundedPrimeSets;

        template<typename T>
        friend class BoundedPrimeSetProducts;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedPrimeSetIterator Begin () const
    {
        BoundedPrimeSetIterator begin (*this);
        begin.n = 1;
        // The first index is 2^64 - 1 for all normal iterators and is incremented to 0 upon reaching the end.
        begin.indices = {std::numeric_limits<size_t>::max ()};
        // The first two "primes" are dummy values for a more efficient increment algorithm.
        begin.primes = {std::numeric_limits<T>::max (), std::numeric_limits<T>::max ()};
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

    template<typename T>
    friend class BoundedFactorizations;

    template<typename T>
    friend class BoundedPrimeSetProducts;
};

template<typename T>
using BPS = BoundedPrimeSets<T>;

template<typename T>
using BPSI = BoundedPrimeSets<T>::BoundedPrimeSetIterator;
