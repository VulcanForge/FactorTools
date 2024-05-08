#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <span>
#include <stdexcept>
#include <vector>

#include <PrimeSieve.h>

// Represents the set of fixed-size sets of primes, drawn from a pool, whose product is less than a given upper bound.
class BoundedPrimeFixedSizeSets
{
private:
    // The upper bound.
    uint64_t upperBound;

    // The number of primes per set.
    uint16_t setSize;

    // The pool from which to draw the sets of primes.
    std::vector<uint64_t> primePool;

public:
    // Constructs a BoundedPrimeFixedSizeSets object with the given upper bound and set size.
    // The pool is constructed as the set of primes < upperBound.
    BoundedPrimeFixedSizeSets (uint64_t upperBound, uint16_t tupleSize)
        : upperBound (upperBound), setSize (tupleSize)
    {
        if (tupleSize == 0)
            throw std::out_of_range ("Empty tuples not implemented.");

        PrimeSieve tempPrimePool (upperBound);
        primePool = std::vector (tempPrimePool.Primes ().cbegin (), tempPrimePool.Primes ().cend ());
        // We require a value >= 'upperBound' at the end (not necessarily prime) to avoid expensive range checking.
        primePool.emplace_back (upperBound);

        if (tupleSize > primePool.size ())
            throw std::out_of_range ("Not enough primes in this range.");
    }

    // Constructs a BoundedPrimeFixedSizeSets object with the given upper bound and set size.
    BoundedPrimeFixedSizeSets (uint64_t upperBound, uint16_t setSize, const std::vector<uint64_t> primePool)
        : upperBound (upperBound), setSize (setSize)
    {
        if (setSize == 0)
            throw std::out_of_range ("Empty tuples not implemented.");

        this->primePool = std::vector (primePool);
        // We require a value >= 'upperBound' at the end (not necessarily prime) to avoid expensive range checking.
        this->primePool.emplace_back (upperBound);

        if (setSize > primePool.size ())
            throw std::out_of_range ("Not enough primes in this range.");
    }

    // Represents an iterator over the sets of primes represented by a given BoundedPrimeFixedSizeSets object.
    class BoundedPrimeFixedSizeSetIterator
    {
    private:
        // The BoundedPrimeFixedSizeSets object.
        const BoundedPrimeFixedSizeSets& parent;

        // The product of the current set of primes.
        uint64_t n;

        // The indices in 'parent.primePool' of the primes in the current set.
        std::vector<size_t> indices;

        // The current set of primes.
        std::vector<uint64_t> primes;

        // Constructs a BoundedPrimeFixedSizeSetIterator with the given parent.
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeFixedSizeSets.
        BoundedPrimeFixedSizeSetIterator (const BoundedPrimeFixedSizeSets& parent)
            : parent (parent) {}

    public:
        // Returns the current set of primes.
        std::span<const uint64_t> Primes () const
        {
            return std::span (primes.cbegin (), primes.cend ());
        }

        // Returns the product of the current set of primes.
        uint64_t N () const
        {
            return n;
        }

        BoundedPrimeFixedSizeSetIterator& operator++ ()
        {
            // The index of the prime index to attempt to increment.
            size_t toIncrement = indices.size ();

            do
            {
                toIncrement--;
                std::iota (indices.begin () + toIncrement, indices.end (), indices[toIncrement] + 1);

                for (size_t i = toIncrement; i < indices.size (); i++)
                    primes[i] = parent.primePool[indices[i]];

                n = std::accumulate (primes.cbegin (), primes.cend (), 1ULL, std::multiplies<uint64_t> ());
            } while (n >= parent.upperBound && toIncrement != 0);

            if (n >= parent.upperBound)
                n = 0;

            return *this;
        }

        BoundedPrimeFixedSizeSetIterator operator++ (int)
        {
            BoundedPrimeFixedSizeSetIterator old = *this;
            operator++ ();
            return old;
        }

        // Returns the value of the Moebius function at 'n'.
        int16_t MoebiusN () const
        {
            // Efficient (-1)^n algorithm.
            return (-(parent.setSize & 1)) | 1;
        }

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedPrimeFixedSizeSetIterator& left, const BoundedPrimeFixedSizeSetIterator& right)
        {
            return left.n == right.n;
        }

        friend class BoundedPrimeFixedSizeSets;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedPrimeFixedSizeSetIterator Begin () const
    {
        BoundedPrimeFixedSizeSetIterator begin (*this);
        begin.indices = std::vector<uint64_t> (setSize);
        // Start with the first 'setSize' primes.
        std::iota (begin.indices.begin (), begin.indices.end (), 0ULL);
        begin.primes = std::vector<uint64_t> (primePool.cbegin (), primePool.cbegin () + setSize);
        // Multiply the first 'setSize' primes.
        begin.n = std::accumulate (begin.primes.cbegin (), begin.primes.cend (), 1ULL, std::multiplies<uint64_t> ());

        // If the 'setSize'-th primorial is too large, return the end iterator.
        if (begin.n >= upperBound)
            begin.n = 0;

        return begin;
    }

    // Returns an iterator to the end of the prescribed set.
    BoundedPrimeFixedSizeSetIterator End () const
    {
        BoundedPrimeFixedSizeSetIterator end (*this);
        end.n = 0;
        // There is no need to initialize 'primes' as it is not considered in iterator comparison.
        return end;
    }
};

using BPFSS = BoundedPrimeFixedSizeSets;

using BPFSSI = BoundedPrimeFixedSizeSets::BoundedPrimeFixedSizeSetIterator;
