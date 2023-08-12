#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
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
template<std::unsigned_integral T = uint64_t>
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

    // Returns the 'prime'-adic valuation of 'n'.
    uint16_t P_AdicValuation (T prime) const
    {
        for (T primePower : primeFactors)
            if (primePower.prime == prime)
                return primePower.power;

        return 0;
    }

    // Returns the 'prime'-adic valuation of 'n'.
    uint16_t NuP (T prime) const
    {
        return P_AdicValuation (prime);
    }

    // Returns the number of distinct prime factors of 'n'.
    size_t SmallOmega () const
    {
        return primeFactors->size ();
    }

    // Returns the sum of the exponents of the prime power divisors of 'n'.
    size_t BigOmega () const
    {
        size_t sum = 0;

        for (PrimePower primePower : *primeFactors)
        {
            sum += primePower.power;
        }

        return sum;
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

    // Returns the number of factors of 'n'.
    size_t Tau () const
    {
        return factors->size ();
    }

    // Returns the sum of the proper factors of 'n'.
    uint64_t SumProperFactors () const
    {
        return std::accumulate (FactorsBegin (), FactorsEnd () - 1, 0ULL);
    }

    // Returns the sum of the divisors of 'n'.
    uint64_t Sigma1 () const
    {
        return std::accumulate (FactorsBegin (), FactorsEnd (), 0ULL);
    }

    // Returns the sum of the 'k'-th powers of the divisors of 'n'.
    uint64_t SigmaK (uint16_t k) const
    {
        uint64_t sum = 0;

        for (T factor : *factors)
            sum += Pow (uint64_t (factor), k);

        return sum;
    }

    // Returns the number of integers in [0, 'n'] coprime to 'n'.
    T Totient () const
    {
        T totient = n;

        for (PrimePower primePower : *primeFactors)
            totient = (totient / primePower.prime) * (primePower.prime - 1);

        return totient;
    }

    // Returns the number of integers in [0, 'n'] coprime to 'n'.
    T EulerPhi () const
    {
        return Totient ();
    }

    // Returns the radical of 'n'.
    T Radical () const
    {
        T radical = 1;

        for (PrimePower primePower : *primeFactors)
            radical *= primePower.prime;

        return radical;
    }

    // Returns 0 if 'n' is not squarefree, 1 if 'n' has an even number of prime factors, and -1 otherwise.
    int16_t MoebiusFunction () const
    {
        int16_t mu = 1;

        for (PrimePower primePower : *primeFactors)
        {
            if (primePower.power > 1)
                return 0;
            else
                mu *= -1;
        }

        return mu;
    }

    // Returns 0 if 'n' is not squarefree, 1 if 'n' has an even number of prime factors, and -1 otherwise.
    int16_t Mu () const
    {
        return MoebiusFunction ();
    }

    // Returns -1 to the power of 'BigOmega ()'.
    int16_t LiouvilleFunction () const
    {
        return (BigOmega () % 2 == 0) ? 1 : -1;
    }

    // Returns -1 to the power of 'BigOmega ()'.
    int16_t SmallLambda () const
    {
        return (BigOmega () % 2 == 0) ? 1 : -1;
    }

    // Returns the least common multiple of the multiplicative orders of the integers in [0, 'n'] coprime to 'n'.
    T CarmichaelFunction () const
    {
        if (n == 1)
            return 1;

        T exponent;

        if ((*primeFactors)[0].prime == 2)
        {
            if ((*primeFactors)[0].power == 1)
                exponent = 1;
            else if ((*primeFactors)[0].power == 2)
                exponent = 2;
            else
                exponent = Pow (2ULL, (*primeFactors)[0].power - 2);
        }

        for (auto primePower = ++primeFactors->cbegin (), primePowersEnd = primeFactors->cend (); primePower != primePowersEnd; primePower++)
            exponent = std::lcm (n, Pow (uint64_t (primePower->prime), primePower->power - 1) * (primePower->prime - 1));

        return exponent;
    }

    // Returns the greatest common divisor of 'n' and 'other.n'.
    T GCD (const Factorization& other)
    {
        return std::gcd (n, other.n);
    }

    // Returns the lowest common multiple of 'n' and 'other.n'.
    uint64_t LCM (const Factorization& other)
    {
        return std::lcm (n, other.n);
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

    // Returns whether 'n' is coprime to 'other.n'.
    bool IsCoprime (const Factorization& other)
    {
        return GCD (other) == 1;
    }

    // Returns whether 'n' is squarefree.
    bool IsSquarefree () const
    {
        for (PrimePower primePower : primeFactors)
            if (primePower.power > 1)
                return false;

        return true;
    }

    // Returns whether 'n' is 'h'-free; that is, whether no 'h'-th power divides 'n'.
    bool IsHFree (uint16_t h) const
    {
        for (PrimePower primePower : primeFactors)
            if (primePower.power >= h)
                return false;

        return true;
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
