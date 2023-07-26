#pragma once

#include <cstdint>
#include <vector>

#include "BoundedPrimeSets.h"
#include "PrimePower.h"

// Represents the set of numbers whose prime factors are drawn from a given set and which are less than a given upper bound.
class BoundedFactorizations
{
private:
    // A helper object for constructing factorizations.
    BPS bps;

public:
    // Constructs a BoundedFactorizations object with the given upper bound.
    BoundedFactorizations (uint64_t upperBound);

    // Represents an iterator over the set of numbers represented by a given BoundedFactorizations object.
    class BoundedFactorizationIterator
    {
    private:
        // The BoundedFactorizations object.
        const BoundedFactorizations& parent;
        // The current number.
        uint64_t n;
        // A helper object for constructing factorizations.
        BPSI bpsi;
        // The prime factorization of 'n'.
        std::vector<PrimePower> factorization;

        // Constructs a BoundedFactorizationIterator object with the given parent and "set of primes".
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeSets.
        BoundedFactorizationIterator (const BoundedFactorizations& parent, const BPSI& bpsi);

    public:
        // Returns an iterator to the beginning of the current prime factorization.
        std::vector<PrimePower>::const_iterator FactorizationBegin () const;
        // Returns an iterator to the end of the current prime factorization.
        std::vector<PrimePower>::const_iterator FactorizationEnd () const;

        // Returns the current number.
        uint64_t N () const;

        BoundedFactorizationIterator& operator++ ();
        BoundedFactorizationIterator operator++ (int);

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedFactorizationIterator& left, const BoundedFactorizationIterator& right);

        friend class BoundedFactorizations;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedFactorizationIterator Begin () const;
    // Returns an iterator to the end of the prescribed set.
    BoundedFactorizationIterator End () const;
};

using BF = BoundedFactorizations;
using BFI = BoundedFactorizations::BoundedFactorizationIterator;
