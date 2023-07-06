#include "OptimizedFactorization.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

#include "Exponent.h"
#include "PrimeSieve.h"

std::optional<PrimeSieve> OptimizedFactorization::cache;

void OptimizedFactorization::GeneratePrimeFactors (bool verbose)
{
    uint64_t sqrt_r = uint64_t (sqrt (n));

    if (!cache || cache->Limit () <= sqrt_r)
        InitializeCache (sqrt_r + 1, verbose);

    auto begin = cache->ListBegin ();
    auto end = cache->ListEnd ();
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

void OptimizedFactorization::GenerateFactors ()
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

        factors->push_back (factor);

        for (size_t j = 0; j < addressSize; j++)
        {
            address[j] = (address[j] + 1) % ((*primeFactors)[j].power + 1);

            if (address[j] > 0)
                break;
        }
    }

    std::sort (factors->begin (), factors->end ());
}

OptimizedFactorization::OptimizedFactorization (uint64_t n)
    : OptimizedFactorization (n, false) {}

OptimizedFactorization::OptimizedFactorization (uint64_t n, bool verbose)
    : n (n), primeFactors (std::make_shared <std::vector<PrimePower>> ()), factors (std::make_shared<std::vector<uint64_t>> ())
{
    GeneratePrimeFactors (verbose);
    GenerateFactors ();
}

std::shared_ptr<const std::vector<PrimePower>> OptimizedFactorization::PrimeFactors () const
{
    return primeFactors;
}

std::vector<PrimePower>::const_iterator OptimizedFactorization::PrimeFactorsBegin () const
{
    return primeFactors->cbegin ();
}

std::vector<PrimePower>::const_iterator OptimizedFactorization::PrimeFactorsEnd () const
{
    return primeFactors->cend ();
}

size_t OptimizedFactorization::PrimeFactorsCount () const
{
    return primeFactors->size ();
}

std::shared_ptr<const std::vector<uint64_t>> OptimizedFactorization::Factors () const
{
    return factors;
}

std::vector<uint64_t>::const_iterator OptimizedFactorization::FactorsBegin () const
{
    return factors->cbegin ();
}

std::vector<uint64_t>::const_iterator OptimizedFactorization::FactorsEnd () const
{
    return factors->cend ();
}

size_t OptimizedFactorization::FactorsCount () const
{
    return factors->size ();
}

uint64_t OptimizedFactorization::SumProperFactors () const
{
    return std::accumulate (FactorsBegin (), FactorsEnd () - 1, 0ULL);
}

bool OptimizedFactorization::IsPrime () const
{
    return FactorsCount () == 2;
}

bool OptimizedFactorization::IsComposite () const
{
    return FactorsCount () > 2;
}

bool OptimizedFactorization::IsPerfect () const
{
    return SumProperFactors () == n;
}

bool OptimizedFactorization::IsDeficient () const
{
    return SumProperFactors () < n;
}

bool OptimizedFactorization::IsAbundant () const
{
    return SumProperFactors () > n;
}

void OptimizedFactorization::InitializeCache (size_t limit, bool verbose)
{
    cache = std::make_optional<PrimeSieve> (limit, verbose);
}
