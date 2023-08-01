#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include "Exponent.h"
#include "PrimePower.h"
#include "PrimeSieve.h"

// A factorization of a natural number.
template<typename T = uint64_t>
class Factorization
{
private:
    // The natural number.
    T n;

    // The prime factors of 'n'.
    std::shared_ptr<std::vector<PrimePower<T, uint16_t>>> primeFactors;

    // The factors of 'n'.
    std::shared_ptr<std::vector<T>> factors;

    // Computes the prime factors of 'n' and optionally outputs progress to 'clog'.
    void GeneratePrimeFactors (std::shared_ptr<PrimeSieve<T>> sieve, bool verbose)
    {
        T sqrt_r = sqrt (n);
        T r = n;

        if (verbose)
        {
            std::clog << "\n";

            for (auto prime = sieve->PrimesBegin (), primesEnd = sieve->PrimesEnd (); prime != primesEnd && *prime <= sqrt_r; prime++)
            {
                uint16_t power = 0;

                std::clog << "Trial factoring by " << *prime << "\n";

                while (r % *prime == 0)
                {
                    r /= *prime;
                    power++;
                }

                if (power > 0)
                {
                    primeFactors->emplace_back (*prime, power);
                    sqrt_r = sqrt (r);
                }
            }
        }
        else
            for (auto prime = sieve->PrimesBegin (), primesEnd = sieve->PrimesEnd (); prime != primesEnd && *prime <= sqrt_r; prime++)
            {
                uint16_t power = 0;

                while (r % *prime == 0)
                {
                    r /= *prime;
                    power++;
                }

                if (power > 0)
                {
                    primeFactors->emplace_back (*prime, power);
                    sqrt_r = sqrt (r);
                }
            }

        if (r > 1)
            primeFactors->emplace_back (r, 1);
    }

    // Computes the factors of 'n'.
    void GenerateFactors ()
    {
        size_t addressSize = primeFactors->size ();
        std::vector<size_t> address (addressSize, 0);
        size_t numberFactors = 1;

        for (size_t i = 0; i < addressSize; i++)
            numberFactors *= ((*primeFactors)[i].power + 1);

        for (size_t i = 0; i < numberFactors; i++)
        {
            T factor = 1;

            for (size_t j = 0; j < addressSize; j++)
                factor *= Pow (uint64_t ((*primeFactors)[j].prime), address[j]);

            factors->emplace_back (factor);

            for (size_t j = 0; j < addressSize; j++)
            {
                address[j] = (address[j] + 1) % ((*primeFactors)[j].power + 1);

                if (address[j] > 0)
                    break;
            }
        }

        std::sort (factors->begin (), factors->end ());
    }

public:
    // Constructs a Factorization of 'n' and optionally outputs progress to 'clog'.
    Factorization (T n, bool verbose = false)
        : Factorization (n, std::make_shared <PrimeSieve<T>> (sqrt (n))) {}

    // Constructs a Factorization of 'n' using a precomputed list of primes
    // and optionally outputs progress to 'clog'.
    Factorization (T n, std::shared_ptr<PrimeSieve<T>> sieve, bool verbose = false)
        : n (n), primeFactors (std::make_shared <std::vector<PrimePower<T, uint16_t>>> ()), factors (std::make_shared<std::vector<T>> ())
    {
        GeneratePrimeFactors (sieve, verbose);
        GenerateFactors ();
    }

    // Returns the list of prime factors of 'n'.
    std::shared_ptr<const std::vector<PrimePower<T, uint16_t>>> PrimeFactors () const
    {
        return primeFactors;
    }

    // Returns an iterator to the beginning of the prime factors of 'n'.
    std::vector<PrimePower<T, uint16_t>>::const_iterator PrimeFactorsBegin () const
    {
        return primeFactors->cbegin ();
    }

    // Returns an iterator to the end of the prime factors of 'n'.
    std::vector<PrimePower<T, uint16_t>>::const_iterator PrimeFactorsEnd () const
    {
        return primeFactors->cend ();
    }

    // Returns the number of distinct prime factors of 'n'.
    size_t PrimeFactorsCount () const
    {
        return primeFactors->size ();
    }

    // Returns the list of factors of 'n'.
    std::shared_ptr<const std::vector<T>> Factors () const
    {
        return factors;
    }

    // Returns an iterator to the beginning of the factors of 'n'.
    std::vector<T>::const_iterator FactorsBegin () const
    {
        return factors->cbegin ();
    }

    // Returns an iterator to the end of the prime factors of 'n'.
    std::vector<T>::const_iterator FactorsEnd () const
    {
        return factors->cend ();
    }

    // Returns the number of factors of 'n'.
    size_t FactorsCount () const
    {
        return factors->size ();
    }

    // Returns the sum of the proper factors of 'n'.
    T SumProperFactors () const
    {
        return std::accumulate (FactorsBegin (), FactorsEnd () - 1, T (0));
    }

    // Returns whether 'n' is prime.
    bool IsPrime () const
    {
        return FactorsCount () == 2;
    }

    // Returns whether 'n' is composite.
    bool IsComposite () const
    {
        return FactorsCount () > 2;
    }

    // Returns whether 'n' is perfect (equal to the sum of its proper factors).
    bool IsPerfect () const
    {
        return SumProperFactors () == n;
    }

    // Returns whether 'n' is deficient (less than the sum of its proper factors).
    bool IsDeficient () const
    {
        return SumProperFactors () < n;
    }

    // Returns whether 'n' is abundant (greater than the sum of its proper factors).
    bool IsAbundant () const
    {
        return SumProperFactors () > n;
    }
};
