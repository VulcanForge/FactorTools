#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "PrimeSieve.h"

// Represents the set of d-tuples of primes whose product is less than a given upper bound.
class BoundedPrimeTuples
{
private:
    // The upper bound.
    uint64_t upperBound;

    // The number of primes per tuple.
    uint16_t tupleSize;

    // The primes less than 'upperBound'.
    std::vector<uint64_t> primePool;

public:
    // Constructs a BoundedPrimeTuples object with the given upper bound and tuple size.
    BoundedPrimeTuples (uint64_t upperBound, uint16_t tupleSize)
        : upperBound (upperBound), tupleSize (tupleSize)
    {
        if (tupleSize == 0)
            throw std::out_of_range ("Empty tuples not implemented.");

        // It is convenient to have one prime larger than 'upperBound' to avoid expensive range checking.
        primePool = *(PrimeSieve (upperBound + 1550).Primes ());

        if (tupleSize > primePool.size ())
            throw std::out_of_range ("Not enough primes in this range.");
    }

    // Represents an iterator over the tuples of primes represented by a given BoundedPrimeTuples object.
    class BoundedPrimeTupleIterator
    {
    private:
        // The BoundedPrimeTuples object.
        const BoundedPrimeTuples& parent;

        // The product of the current tuple of primes.
        uint64_t n;

        // The indices in 'parent.primePool' of the primes in the current tuple.
        std::vector<size_t> indices;

        // The current tuple of primes.
        std::vector<uint64_t> primes;

        // Constructs a BoundedPrimeTupleIterator with the given parent.
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeTuples.
        BoundedPrimeTupleIterator (const BoundedPrimeTuples& parent)
            : parent (parent) {}

    public:
        // Returns an iterator to the beginning of the current tuple of primes.
        std::vector<uint64_t>::const_iterator PrimesBegin () const
        {
            return primes.cbegin ();
        }

        // Returns an iterator to the end of the current tuple of primes.
        std::vector<uint64_t>::const_iterator PrimesEnd () const
        {
            return primes.cend ();
        }

        // Returns the product of the current tuple of primes.
        uint64_t N () const
        {
            return n;
        }

        BoundedPrimeTupleIterator& operator++ ()
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

        BoundedPrimeTupleIterator operator++ (int)
        {
            BoundedPrimeTupleIterator old = *this;
            operator++ ();
            return old;
        }

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedPrimeTupleIterator& left, const BoundedPrimeTupleIterator& right)
        {
            return left.n == right.n;
        }

        friend class BoundedPrimeTuples;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedPrimeTupleIterator Begin () const
    {
        BoundedPrimeTupleIterator begin (*this);
        begin.indices = std::vector<uint64_t> (tupleSize);
        // Start with the first 'tupleSize' primes.
        std::iota (begin.indices.begin (), begin.indices.end (), 0ULL);
        begin.primes = std::vector<uint64_t> (primePool.cbegin (), primePool.cbegin () + tupleSize);
        // Multiply the first 'tupleSize' primes.
        begin.n = std::accumulate (begin.primes.cbegin (), begin.primes.cend (), 1ULL, std::multiplies<uint64_t> ());

        // If the 'd'-th primorial is too large, return the end iterator.
        if (begin.n >= upperBound)
            begin.n = 0;

        return begin;
    }

    // Returns an iterator to the end of the prescribed set.
    BoundedPrimeTupleIterator End () const
    {
        BoundedPrimeTupleIterator end (*this);
        end.n = 0;
        // There is no need to initialize 'primes' as it is not considered in iterator comparison.
        return end;
    }
};

using BPT = BoundedPrimeTuples;

using BPTI = BoundedPrimeTuples::BoundedPrimeTupleIterator;
