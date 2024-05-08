#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include <BoundedPrimeSets.h>
#include <Exponent.h>
#include <PrimePower.h>

// Represents the set of numbers, whose prime factors lie in a given set, and which are less than a given upper bound.
class BoundedPrimeSetProducts
{
private:
    // A helper object storing the set of primes.
    BPSI bpsi;

public:
    // Constructs a BoundedPrimeSetProducts object with the given "set of primes".
    BoundedPrimeSetProducts (const BPSI& bpsi)
        : bpsi (bpsi) {}

    // Represents an iterator over the numbers represented by a given BoundedPrimeSetProducts object.
    class BoundedPrimeSetProductIterator
    {
    private:
        // The BoundedPrimeSetProducts object.
        const BoundedPrimeSetProducts& parent;

        // The current number.
        uint64_t n;

        // The prime factorization of 'n'.
        std::vector<PrimePower<>> factorization;

        // Constructs a BoundedPrimeSetProductIterator object with the given parent.
        // Most construction is performed in the factory 'Begin' and 'End' methods of BoundedPrimeSetProducts.
        BoundedPrimeSetProductIterator (const BoundedPrimeSetProducts& parent)
            : parent (parent) {}

    public:
        // Returns the current prime factorization.
        std::span<const PrimePower<>> Factorization () const
        {
            return std::span (factorization.cbegin (), factorization.cend ());
        }

        // Returns the current number.
        uint64_t N () const
        {
            return n;
        }

        BoundedPrimeSetProductIterator& operator++ ()
        {
            // Algorithm: Attempt to increment the power of the last prime in the factorization.
            // If that is too large, repeatedly reset that power to 1 and attempt to increment the power of the previous prime.
            // 
            // The index of the prime power to attempt to increment.
            size_t index = factorization.size () - 1;

            // Attempt to increment the prime power at 'index' (and end the loop).
            while (index != SIZE_MAX && n * factorization[index].prime >= parent.bpsi.parent.upperBound)
            {
                n /= Pow (factorization[index].prime, factorization[index].power - 1);
                factorization[index].power = 1;
                // If 'index' is 0, it will become UINT64_MAX and be > 'factorization.size ()'.
                // Then the loop will terminate,
                // the next conditional will recognize that all exponent tuples have been exhausted,
                // and the prime set itself will "increment".
                index--;
            }

            // Actually increment the prime power at 'index', unless all exponent tuples for the current prime set have been exhausted.
            if (index != SIZE_MAX)
            {
                n *= factorization[index].prime;
                factorization[index].power++;
                return *this;
            }

            // All exponent tuples for the current prime set have been exhausted.
            // Move into the end iterator state.
            n = 0;
            return *this;
        }

        BoundedPrimeSetProductIterator operator++ (int)
        {
            BoundedPrimeSetProductIterator old = *this;
            operator++ ();
            return old;
        }

        // Returns the value of the Moebius function at 'n'.
        int16_t MoebiusN () const
        {
            for (auto primePower = factorization.cbegin (); primePower != factorization.cend (); primePower++)
                if (primePower->power > 1)
                    return 0;

            // Efficient (-1)^n algorithm.
            return (-(factorization.size () & 1)) | 1;
        }

        // WARNING: No comparison of parent object is performed.
        friend bool operator== (const BoundedPrimeSetProductIterator& left, const BoundedPrimeSetProductIterator& right)
        {
            return left.n == right.n;
        }

        friend class BoundedPrimeSetProducts;
    };

    // Returns an iterator to the beginning of the prescribed set.
    BoundedPrimeSetProductIterator Begin () const
    {
        BoundedPrimeSetProductIterator begin (*this);
        begin.n = 1;

        // Initialize all exponents to 1.
        for (uint64_t prime : bpsi.Primes ())
        {
            begin.factorization.emplace_back (prime, 1);
            begin.n *= prime;
        }

        return begin;
    }

    // Returns an iterator to the end of the prescribed set.
    BoundedPrimeSetProductIterator End () const
    {
        BoundedPrimeSetProductIterator end (*this);
        end.n = 0;
        // There is no need to initialize 'factorization' as it is not considered in iterator comparison.
        return end;
    }
};

using BPSP = BoundedPrimeSetProducts;

using BPSPI = BPSP::BoundedPrimeSetProductIterator;
