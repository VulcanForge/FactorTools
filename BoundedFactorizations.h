#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "BoundedPrimeSets.h"
#include "PrimePower.h"

// Represents the set of numbers whose prime factors are drawn from a given set and which are less than a given upper bound.
template<typename T = uint64_t>
class BoundedFactorizations
{
private:
    // A helper object for constructing factorizations.
    BPS<T> bps;

public:
    // Constructs a BoundedFactorizations object with the given upper bound.
    BoundedFactorizations (T upperBound)
        : bps (upperBound) {}

    // Represents an iterator over the set of numbers represented by a given BoundedFactorizations object.
    class BoundedFactorizationIterator
    {
    private:
        // The BoundedFactorizations object.
        const BoundedFactorizations& parent;

        // The current number.
        T n;

        // A helper object for constructing factorizations.
        BPSI<T> bpsi;

        // The prime factorization of 'n'.
        std::vector<PrimePower<T, uint16_t>> factorization;

        // Constructs a BoundedFactorizationIterator object with the given parent and "set of primes".
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeSets.
        BoundedFactorizationIterator (const BoundedFactorizations& parent, const BPSI<T>& bpsi)
            : parent (parent), bpsi (bpsi) {}

    public:
        // Returns an iterator to the beginning of the current prime factorization.
        std::vector<PrimePower<T, uint16_t>>::const_iterator FactorizationBegin () const
        {
            return factorization.cbegin ();
        }

        // Returns an iterator to the end of the current prime factorization.
        std::vector<PrimePower<T, uint16_t>>::const_iterator FactorizationEnd () const
        {
            return factorization.cend ();
        }

        // Returns the current number.
        T N () const
        {
            return n;
        }

        BoundedFactorizationIterator& operator++ ()
        {
            // Algorithm: Attempt to increment the power of the last prime in the factorization.
            // If that is too large, repeatedly reset that power to 1 and attempt to increment the power of the previous prime.
            // If all exponent tuples for the current prime set have been exhausted, "increment" the prime set.

            // The index of the prime power to attempt to increment.
            size_t index = factorization.size () - 1;

            // Attempt to increment the prime power at 'index' (and end the loop).
            while (index < factorization.size () && n * factorization[index].prime >= parent.bps.upperBound)
            {
                n /= Pow (uint64_t (factorization[index].prime), factorization[index].power - 1);
                factorization[index].power = 1;
                // If 'index' is 0, it will become UINT64_MAX and be > 'factorization.size ()'.
                // Then the loop will terminate,
                // the next conditional will recognize that all exponent tuples have been exhausted,
                // and the prime set itself will "increment".
                index--;
            }

            // Actually increment the prime power at 'index', unless all exponent tuples for the current prime set have been exhausted.
            if (index < factorization.size ())
            {
                n *= factorization[index].prime;
                factorization[index].power++;
                return *this;
            }

            // "Increment" the prime set and reset all exponents to 1.
            bpsi++;
            n = bpsi.N ();
            factorization.clear ();

            for (auto prime = bpsi.PrimesBegin (), primesEnd = bpsi.PrimesEnd (); prime != primesEnd; prime++)
                factorization.emplace_back (*prime, 1);

            return *this;
        }

        BoundedFactorizationIterator operator++ (int)
        {
            BoundedFactorizationIterator old = *this;
            operator++ ();
            return old;
        }

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedFactorizationIterator& left, const BoundedFactorizationIterator& right)
        {
            return left.n == right.n;
        }

        friend class BoundedFactorizations;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedFactorizationIterator Begin () const
    {
        BoundedFactorizationIterator begin (*this, bps.Begin ());
        begin.n = 1;
        // 'factorization' starts empty as 'n' = 1.
        return begin;
    }

    // Returns an iterator to the end of the prescribed set.
    BoundedFactorizationIterator End () const
    {
        BoundedFactorizationIterator end (*this, bps.End ());
        // There is no need to initialize 'factorization' as it is not considered in iterator comparison.
        end.n = 0;
        return end;
    }
};

template<typename T>
using BF = BoundedFactorizations<T>;

template<typename T>
using BFI = BoundedFactorizations<T>::BoundedFactorizationIterator;
