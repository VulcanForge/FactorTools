#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
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
    std::vector<T> coprimes;

    // The list of obstructions.
    const std::vector<T>& obstructions;

public:
    // Constructs a CoprimeSieve over ['lowerLimit', 'upperLimit') with the given obstructions
    // and optionally outputs progress to 'clog'.
    CoprimeSieve (T lowerLimit, T upperLimit, const std::vector<T>& obstructions, bool verbose = false)
        : lowerLimit (lowerLimit),
        upperLimit (upperLimit),
        sieve (upperLimit - lowerLimit, true),
        obstructions (obstructions)
    {
        if (verbose)
            for (T obstruction : obstructions)
            {
                std::clog << "Striking out multiples of " << obstruction << "\n";

                T firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

                for (T multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }
        else
            for (T obstruction : obstructions)
            {
                T firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

                for (T multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }

        for (T i = lowerLimit; i < upperLimit; i++)
            if (sieve.Get (i - lowerLimit))
                coprimes.emplace_back (i);
    }

    // Returns the list of numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::span<const T> Coprimes () const
    {
        return std::span (coprimes.cbegin (), coprimes.cend ());
    }

    // Returns the number of numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    size_t Count () const
    {
        return coprimes.size ();
    }

    // Returns whether 'n' is coprime to every element of obstructions, if n is in ['lowerLimit', 'upperLimit').
    bool IsCoprime (T n) const
    {
        return sieve.Get (n - lowerLimit);
    }
};
