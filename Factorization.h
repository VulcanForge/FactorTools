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

// A factorization of a positive integer.
template<std::unsigned_integral T>
class Factorization
{
private:
    // The integer.
    T n;

    // The prime factorization of 'n'.
    std::shared_ptr<std::vector<PrimePower<T, std::uint32_t>>> primeFactors;

    // The factors of 'n' in increasing order.
    std::shared_ptr<std::vector<T>> factors;

    // Computes the prime factors of 'n' and optionally outputs progress to 'clog'.
    void GeneratePrimeFactors (std::shared_ptr<const PrimeSieve<T>> sieve, bool verbose)
    {
        // Standard trial factoring algorithm.
        T sqrt_r = T (std::sqrt (n));
        T r = n;

        if (verbose)
        {
            std::clog << "\n";

            for (T prime : *sieve->Primes ())
            {
                if (prime > sqrt_r)
                    break;

                std::uint32_t power = 0;

                std::clog << "Trial factoring by " << prime << "\n";

                while (r % prime == 0)
                {
                    r /= prime;
                    ++power;
                }

                if (power > 0)
                {
                    primeFactors->emplace_back (prime, power);
                    sqrt_r = T (std::sqrt (r));
                }
            }
        }
        else
            for (T prime : *sieve->Primes ())
            {
                if (prime > sqrt_r)
                    break;

                std::uint32_t power = 0;

                while (r % prime == 0)
                {
                    r /= prime;
                    ++power;
                }

                if (power > 0)
                {
                    primeFactors->emplace_back (prime, power);
                    sqrt_r = T (std::sqrt (r));
                }
            }

        if (r > 1)
            primeFactors->emplace_back (r, 1);
    }

    // Computes the factors of 'n'.
    void GenerateFactors ()
    {
        std::size_t addressSize = primeFactors->size ();
        std::vector<std::size_t> address (addressSize, 0);
        std::size_t numberFactors = 1;

        // Standard product form of divisor counting function.
        for (std::size_t i = 0; i < addressSize; ++i)
            numberFactors *= ((*primeFactors)[i].power + 1);

        // Iterate through all possible exponent tuples on the primes dividing 'n'.
        for (std::size_t i = 0; i < numberFactors; ++i)
        {
            T factor = 1;

            for (std::size_t j = 0; j < addressSize; ++j)
                factor *= Pow (std::uint64_t ((*primeFactors)[j].prime), address[j]);

            factors->emplace_back (factor);

            // Move to the next exponent tuple in lex order.
            for (std::size_t j = 0; j < addressSize; ++j)
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
        : Factorization (n, std::make_shared<const PrimeSieve<T>> (T (std::sqrt (n)), verbose), verbose) {}

    // Constructs a Factorization of 'n' using a precomputed list of primes
    // and optionally outputs progress to 'clog'.
    Factorization (T n, std::shared_ptr<const PrimeSieve<T>> sieve, bool verbose = false)
        : n (n),
        primeFactors (std::make_shared<std::vector<PrimePower<T, std::uint32_t>>> ()),
        factors (std::make_shared<std::vector<T>> ())
    {
        GeneratePrimeFactors (sieve, verbose);
        GenerateFactors ();
    }

    // Returns the prime factorization of 'n'.
    std::shared_ptr<const std::vector<PrimePower<T, std::uint32_t>>> PrimeFactors () const
    {
        return primeFactors;
    }

    // Returns the number of distinct prime factors of 'n'.
    std::size_t PrimeFactorsCount () const
    {
        return primeFactors->size ();
    }

    // Returns the 'prime'-adic valuation of 'n'.
    std::uint32_t PAdicValuation (T prime) const
    {
        for (const auto& primePower : *primeFactors)
            if (primePower.prime == prime)
                return primePower.power;

        return 0;
    }

    // Returns the 'prime'-adic valuation of 'n'.
    std::uint32_t NuP (T prime) const
    {
        return PAdicValuation (prime);
    }

    // Returns the number of distinct prime factors of 'n'.
    std::size_t SmallOmega () const
    {
        return primeFactors->size ();
    }

    // Returns the number of prime factors of 'n' with multiplicity.
    std::size_t BigOmega () const
    {
        std::size_t sum = 0;

        for (const auto& primePower : *primeFactors)
            sum += primePower.power;

        return sum;
    }

    // Returns the factors of 'n' in increasing order.
    std::shared_ptr<const std::vector<T>> Factors () const
    {
        return factors;
    }

    // Returns the number of factors of 'n'.
    std::size_t FactorsCount () const
    {
        return factors->size ();
    }

    // Returns the number of factors of 'n'.
    std::size_t Tau () const
    {
        return FactorsCount ();
    }

    // Returns the sum of the proper factors of 'n'.
    std::uint64_t SumProperFactors () const
    {
        std::uint64_t sum = 0;

        for (auto factor = factors->cbegin (); factor != factors->cend () - 1; ++factor)
            sum += (*factor);

        return sum;
    }

    // Returns the sum of the divisors of 'n'.
    std::uint64_t Sigma1 () const
    {
        return SumProperFactors () + n;
    }

    // Returns the sum of the 'k'-th powers of the divisors of 'n'.
    std::uint64_t SigmaK (std::uint64_t k) const
    {
        std::uint64_t sum = 0;

        for (T factor : *factors)
            sum += Pow (std::uint64_t (factor), k);

        return sum;
    }

    // Returns the number of integers in [0, 'n') coprime to 'n'.
    T Totient () const
    {
        // Standard product representation of totient function.
        T totient = n;

        for (const auto& primePower : *primeFactors)
            totient = (totient / primePower.prime) * (primePower.prime - 1);

        return totient;
    }

    // Returns the number of integers in [0, 'n') coprime to 'n'.
    T EulerPhi () const
    {
        return Totient ();
    }

    // Returns the radical of 'n'.
    T Radical () const
    {
        T radical = 1;

        for (const auto& primePower : *primeFactors)
            radical *= primePower.prime;

        return radical;
    }

    // Returns 0 if 'n' is not squarefree, 1 if 'n' has an even number of prime factors, and -1 otherwise.
    std::int32_t MoebiusFunction () const
    {
        for (const auto& primePower : *primeFactors)
            if (primePower.power > 1)
                return 0;

        // Efficient (-1)^n algorithm.
        return (-(primeFactors->size () & 1)) | 1;
    }

    // Returns 0 if 'n' is not squarefree, 1 if 'n' has an even number of prime factors, and -1 otherwise.
    std::int32_t Mu () const
    {
        return MoebiusFunction ();
    }

    // Returns -1 to the power of 'BigOmega ()'.
    std::int32_t LiouvilleFunction () const
    {
        // Efficient (-1)^n algorithm.
        return (-(BigOmega () & 1)) | 1;
    }

    // Returns -1 to the power of 'BigOmega ()'.
    std::int32_t SmallLambda () const
    {
        // Efficient (-1)^n algorithm.
        return (-(BigOmega () & 1)) | 1;
    }

    // Returns the least common multiple of the multiplicative orders of the integers in [0, 'n') coprime to 'n'.
    T CarmichaelFunction () const
    {
        // Standard product representation of Carmichael function.
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

            for (auto primePower = primeFactors->cbegin () + 1; primePower != primeFactors->cend (); ++primePower)
                exponent = std::lcm
                (
                    exponent,
                    Pow (std::uint64_t (primePower->prime), primePower->power - 1) * (primePower->prime - 1)
                );
        }
        else
        {
            exponent = 1;

            for (const auto& primePower : *primeFactors)
                exponent = std::lcm
                (
                    exponent,
                    Pow (std::uint64_t (primePower.prime), primePower.power - 1) * (primePower.prime - 1)
                );
        }

        return exponent;
    }

    // Returns the greatest common divisor of 'n' and 'other.n'.
    T GCD (const Factorization& other)
    {
        return std::gcd (n, other.n);
    }

    // Returns the lowest common multiple of 'n' and 'other.n'.
    std::uint64_t LCM (const Factorization& other)
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

    // Returns whether 'n' is 'h'-free; that is, whether no 'h'-th power divides 'n'.
    bool IsHFree (std::uint32_t h) const
    {
        for (const auto& primePower : *primeFactors)
            if (primePower.power >= h)
                return false;

        return true;
    }

    // Returns whether 'n' is squarefree.
    bool IsSquarefree () const
    {
        return IsHFree (2);
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
