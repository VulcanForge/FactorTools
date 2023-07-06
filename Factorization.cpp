#include "Factorization.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

#include "Exponent.h"
#include "PrimeSieve.h"

void Factorization::GeneratePrimeFactors (bool verbose)
{
    uint64_t sqrt_r = uint64_t (sqrt (n));
    PrimeSieve sieve (size_t (sqrt_r + 1), verbose);
    auto begin = sieve.ListBegin ();
    auto end = sieve.ListEnd ();
    uint64_t r = n;
    uint64_t prime = 2;

    if (verbose)
    {
        std::clog << "\n";

        for (auto i = begin; i != end && prime <= sqrt_r; i++)
        {
            prime = *i;
            uint64_t power = 0;

            std::clog << "Trial factoring by " << prime << "\n";

            while (r % prime == 0)
            {
                r /= prime;
                power++;
            }

            if (power > 0)
            {
                primeFactors->emplace_back (prime, power);
                sqrt_r = uint64_t (sqrt (r));
            }
        }
    }
    else
        for (auto i = begin; i != end && prime <= sqrt_r; i++)
        {
            prime = *i;
            uint64_t power = 0;

            while (r % prime == 0)
            {
                r /= prime;
                power++;
            }

            if (power > 0)
            {
                primeFactors->emplace_back (prime, power);
                sqrt_r = uint64_t (sqrt (r));
            }
        }

    if (r > 1)
        primeFactors->emplace_back (r, 1);
}

void Factorization::GenerateFactors ()
{
    size_t addressSize = primeFactors->size ();
    std::vector<size_t> address (addressSize, 0);
    size_t numberFactors = 1;

    for (size_t i = 0; i < addressSize; i++)
        numberFactors *= ((*primeFactors)[i].power + 1);

    for (size_t i = 0; i < numberFactors; i++)
    {
        uint64_t factor = 1;

        for (size_t j = 0; j < addressSize; j++)
            factor *= Pow ((*primeFactors)[j].prime, address[j]);

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

Factorization::Factorization (uint64_t n)
    : Factorization (n, false) {}

Factorization::Factorization (uint64_t n, bool verbose)
    : n (n), primeFactors (std::make_shared <std::vector<PrimePower>> ()), factors (std::make_shared<std::vector<uint64_t>> ())
{
    GeneratePrimeFactors (verbose);
    GenerateFactors ();
}

std::shared_ptr<const std::vector<PrimePower>> Factorization::PrimeFactors () const
{
    return primeFactors;
}

std::vector<PrimePower>::const_iterator Factorization::PrimeFactorsBegin () const
{
    return primeFactors->cbegin ();
}

std::vector<PrimePower>::const_iterator Factorization::PrimeFactorsEnd () const
{
    return primeFactors->cend ();
}

size_t Factorization::PrimeFactorsCount () const
{
    return primeFactors->size ();
}

std::shared_ptr<const std::vector<uint64_t>> Factorization::Factors () const
{
    return factors;
}

std::vector<uint64_t>::const_iterator Factorization::FactorsBegin () const
{
    return factors->cbegin ();
}

std::vector<uint64_t>::const_iterator Factorization::FactorsEnd () const
{
    return factors->cend ();
}

size_t Factorization::FactorsCount () const
{
    return factors->size ();
}

uint64_t Factorization::SumProperFactors () const
{
    return accumulate (FactorsBegin (), FactorsEnd () - 1, 0ULL);
}

bool Factorization::IsPrime () const
{
    return FactorsCount () == 2;
}

bool Factorization::IsComposite () const
{
    return FactorsCount () > 2;
}

bool Factorization::IsPerfect () const
{
    return SumProperFactors () == n;
}

bool Factorization::IsDeficient () const
{
    return SumProperFactors () < n;
}

bool Factorization::IsAbundant () const
{
    return SumProperFactors () > n;
}
