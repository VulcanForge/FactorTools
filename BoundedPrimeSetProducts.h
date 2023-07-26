#pragma once

#include <cstdint>
#include <vector>

#include "BoundedPrimeSets.h"
#include "PrimePower.h"

// Represents the set of numbers whose prime factors lie in a given set and which are less than a given upper bound.
class BoundedPrimeSetProducts
{
private:
    // A helper object storing the set of primes.
    BPSI bpsi;

public:
    // Constructs a BoundedPrimeSetProducts object with the given "set of primes".
    BoundedPrimeSetProducts (const BPSI& bpsi);

    // Represents an iterator over the numbers represented by a given BoundedPrimeSetProducts object.
    class BoundedPrimeSetProductIterator
    {
    private:
        // The BoundedPrimeSetProducts object.
        const BoundedPrimeSetProducts& parent;
        // The current number.
        uint64_t n;
        // The prime factorization of 'n'.
        std::vector<PrimePower> factorization;

        // Constructs a BoundedPrimeSetProductIterator object with the given parent.
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeSetProducts.
        BoundedPrimeSetProductIterator (const BoundedPrimeSetProducts& parent);

    public:
        // Returns an iterator to the beginning of the current prime factorization.
        std::vector<PrimePower>::const_iterator FactorizationBegin () const;
        // Returns an iterator to the end of the current prime factorization.
        std::vector<PrimePower>::const_iterator FactorizationEnd () const;

        // Returns the current number.
        uint64_t N () const;

        BoundedPrimeSetProductIterator& operator++ ();
        BoundedPrimeSetProductIterator operator++ (int);

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedPrimeSetProductIterator& left, const BoundedPrimeSetProductIterator& right);

        friend class BoundedPrimeSetProducts;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedPrimeSetProductIterator Begin () const;
    // Returns an iterator to the end of the prescribed set.
    BoundedPrimeSetProductIterator End () const;
};

using BPSP = BoundedPrimeSetProducts;
using BPSPI = BPSP::BoundedPrimeSetProductIterator;
