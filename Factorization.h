#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "PrimePower.h"
#include "PrimeSieve.h"

// A factorization of a natural number.
class Factorization
{
private:
    // The natural number.
    uint64_t n;

    // The prime factors of 'n'.
    std::shared_ptr<std::vector<PrimePower>> primeFactors;

    // The factors of 'n'.
    std::shared_ptr<std::vector<uint64_t>> factors;

    // Computes the prime factors of 'n' and optionally outputs progress to 'clog'.
    void GeneratePrimeFactors (std::shared_ptr<PrimeSieve> sieve, bool verbose);

    // Computes the factors of 'n'.
    void GenerateFactors ();

public:
    // Constructs a Factorization of 'n' and optionally outputs progress to 'clog'.
    Factorization (uint64_t n, bool verbose = false);

    // Constructs a Factorization of 'n' using a precomputed list of primes
    // and optionally outputs progress to 'clog'.
    Factorization (uint64_t n, std::shared_ptr<PrimeSieve> sieve, bool verbose = false);

    // Returns the list of prime factors of 'n'.
    std::shared_ptr<const std::vector<PrimePower>> PrimeFactors () const;

    // Returns an iterator to the beginning of the prime factors of 'n'.
    std::vector<PrimePower>::const_iterator PrimeFactorsBegin () const;

    // Returns an iterator to the end of the prime factors of 'n'.
    std::vector<PrimePower>::const_iterator PrimeFactorsEnd () const;

    // Returns the number of distinct prime factors of 'n'.
    size_t PrimeFactorsCount () const;

    // Returns the list of factors of 'n'.
    std::shared_ptr<const std::vector<uint64_t>> Factors () const;

    // Returns an iterator to the beginning of the factors of 'n'.
    std::vector<uint64_t>::const_iterator FactorsBegin () const;

    // Returns an iterator to the end of the prime factors of 'n'.
    std::vector<uint64_t>::const_iterator FactorsEnd () const;

    // Returns the number of factors of 'n'.
    size_t FactorsCount () const;

    // Returns the sum of the proper factors of 'n'.
    uint64_t SumProperFactors () const;

    // Returns whether 'n' is prime.
    bool IsPrime () const;

    // Returns whether 'n' is composite.
    bool IsComposite () const;

    // Returns whether 'n' is perfect (equal to the sum of its proper factors).
    bool IsPerfect () const;

    // Returns whether 'n' is deficient (less than the sum of its proper factors).
    bool IsDeficient () const;

    /// <summary>
    /// Returns whether 'n is abundant (greater than the sum of its proper factors).
    bool IsAbundant () const;
};
