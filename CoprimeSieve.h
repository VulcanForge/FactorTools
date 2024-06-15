#pragma once

#include <concepts>
#include <iostream>
#include <memory>
#include <vector>

#include <BitArray.h>

// An Eratosthenes-type sieve to return all numbers in a given range coprime to a given list of obstructions.
template<std::unsigned_integral T>
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

    // The obstructions.
    std::shared_ptr<const std::vector<T>> obstructions;

public:
    // Constructs a CoprimeSieve over ['lowerLimit', 'upperLimit') with the given obstructions
    // and optionally outputs progress to 'clog'.
    CoprimeSieve
    (
        T lowerLimit,
        T upperLimit,
        std::shared_ptr<const std::vector<T>> obstructions,
        bool verbose = false
    )
        : lowerLimit (lowerLimit),
        upperLimit (upperLimit),
        sieve (upperLimit - lowerLimit, true),
        obstructions (obstructions)
    {
        if (verbose)
            for (T obstruction : obstructions)
            {
                std::clog << "Striking out multiples of " << obstruction << "\n";

                // Sieve out every multiple of 'obstruction' in the range ['lowerLimit', 'upperLimit').
                T firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

                for (T multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }
        else
            for (T obstruction : obstructions)
            {
                T firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

                // Sieve out every multiple of 'obstruction' in the range ['lowerLimit', 'upperLimit').
                for (T multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                    sieve.Reset (multiple - lowerLimit);
            }

        coprimes = std::make_shared<std::vector<T>> ();

        for (T i = lowerLimit; i < upperLimit; ++i)
            if (sieve.Get (i - lowerLimit))
                coprimes->emplace_back (i);
    }

    // Returns the numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::shared_ptr<const std::vector<T>> Coprimes () const
    {
        return coprimes;
    }

    // Returns the number of numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    size_t Count () const
    {
        return coprimes->size ();
    }

    // Returns whether 'n' is coprime to every element of obstructions, if n is in ['lowerLimit', 'upperLimit').
    // Out of range arguments result in undefined behaviour.
    bool IsCoprime (T n) const
    {
        return sieve.Get (n - lowerLimit);
    }
};
