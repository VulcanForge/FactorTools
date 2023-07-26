#pragma once

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
    BoundedPrimeSets (uint64_t upperBound);

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
        BoundedPrimeSetIterator (const BoundedPrimeSets& parent);

    public:
        // Returns an iterator to the beginning of the current set of primes.
        std::vector<uint64_t>::const_iterator PrimesBegin () const;
        // Returns an iterator to the end of the current set of primes.
        std::vector<uint64_t>::const_iterator PrimesEnd () const;

        // Returns the product of the current set of primes.
        uint64_t N () const;

        BoundedPrimeSetIterator& operator++ ();
        BoundedPrimeSetIterator operator++ (int);

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedPrimeSetIterator& left, const BoundedPrimeSetIterator& right);

        friend class BoundedPrimeSets;
        friend class BoundedPrimeSetProducts;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedPrimeSetIterator Begin () const;
    // Returns an iterator to the end of the prescribed set.
    BoundedPrimeSetIterator End () const;

    friend class BoundedFactorizations;
    friend class BoundedPrimeSetProducts;
};

using BPS = BoundedPrimeSets;
using BPSI = BoundedPrimeSets::BoundedPrimeSetIterator;
