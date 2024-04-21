#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <BitArray.h>

// An Eratosthenes-type sieve to return all numbers in a given range coprime to a given list of obstructions.
template<std::unsigned_integral T = uint64_t>
class CoprimeSieve
{
private:
    // The underlying bit array.
    BitArray sieve;

    // The inclusive lower bound on the numbers sieved.
    T lowerLimit;

    // The exclusive upper bound on the numbers sieved.
    T upperLimit;

    // The numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::shared_ptr<std::vector<T>> coprimes;

    // The list of obstructions.
    std::shared_ptr<const std::vector<T>> obstructions;

public:
    // Constructs a CoprimeSieve over ['lowerLimit', 'upperLimit') with the given obstructions
    // and optionally outputs progress to 'clog'.
    CoprimeSieve (T lowerLimit, T upperLimit, std::shared_ptr<const std::vector<T>> obstructions, bool verbose = false)
        : lowerLimit (lowerLimit),
        upperLimit (upperLimit),
        sieve (upperLimit - lowerLimit, true),
        obstructions (obstructions),
        coprimes (std::make_shared<std::vector<T>> ())
    {
        if (verbose)
            for (T obstruction : *obstructions)
            {
                std::clog << "Striking out multiples of " << obstruction << "\n";

                T firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

                for (T multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }
        else
            for (T obstruction : *obstructions)
            {
                T firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

                for (T multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }

        for (T i = lowerLimit; i < upperLimit; i++)
            if (sieve.Get (i - lowerLimit))
                coprimes->emplace_back (i);
    }

    CoprimeSieve
    (
        T lowerLimit,
        T upperLimit,
        std::vector<T>::const_iterator obstructionsBegin,
        std::vector<T>::const_iterator obstructionsEnd,
        bool verbose = false
    )
        : lowerLimit (lowerLimit),
        upperLimit (upperLimit),
        sieve (upperLimit - lowerLimit, true),
        coprimes (std::make_shared<std::vector<T>> ())
    {
        if (verbose)
            for (auto obstruction = obstructionsBegin; obstruction != obstructionsEnd; obstruction++)
            {
                std::clog << "Striking out multiples of " << *obstruction << "\n";

                T firstMultiple = ((lowerLimit + *obstruction - 1) / *obstruction) * *obstruction;

                for (T multiple = firstMultiple; multiple < upperLimit; multiple += *obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }
        else
            for (auto obstruction = obstructionsBegin; obstruction != obstructionsEnd; obstruction++)
            {
                T firstMultiple = ((lowerLimit + *obstruction - 1) / *obstruction) * *obstruction;

                for (T multiple = firstMultiple; multiple < upperLimit; multiple += *obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }

        for (T i = lowerLimit; i < upperLimit; i++)
            if (sieve.Get (i - lowerLimit))
                coprimes->emplace_back (i);
    }

    // Returns the inclusive lower and the exclusive upper bounds on the numbers sieved.
    std::pair<T, T> Limits () const
    {
        return std::pair<T, T> (lowerLimit, upperLimit);
    }

    // Returns the list of numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::shared_ptr<const std::vector<T>> Coprimes () const
    {
        return coprimes;
    }

    // Returns an iterator to the beginning of the numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::vector<T>::const_iterator CoprimesBegin () const
    {
        return coprimes->cbegin ();
    }

    // Returns an iterator to the end of the numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::vector<T>::const_iterator CoprimesEnd () const
    {
        return coprimes->cend ();
    }

    // Returns the list of obstructions.
    // Will be a nullptr if the sieve was constructed from iterators.
    std::shared_ptr<const std::vector<T>> ObstructionList () const
    {
        return obstructions;
    }

    // Returns the number of numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    size_t Count () const
    {
        return coprimes->size ();
    }

    // Returns whether 'n' is coprime to every element of obstructions, if n is in ['lowerLimit', 'upperLimit').
    bool IsCoprime (T n) const
    {
        return sieve.Get (n - lowerLimit);
    }

    // BEGIN STL-COMPATIBILITY FUNCTIONS

    // Indexes 'coprimes'.
    const T& operator[] (size_t index) const
    {
        return (*coprimes)[index];
    }

    // Duplicates of 'CoprimesBegin' and 'CoprimesEnd' for for-each loops.

    std::vector<T>::const_iterator begin () const
    {
        return coprimes->cbegin ();
    }

    std::vector<T>::const_iterator end () const
    {
        return coprimes->cend ();
    }
};
