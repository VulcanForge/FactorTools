#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "PrimePower.h"
#include "PrimeSieve.h"

/// <summary>
/// A factorization of a non-negative integer n.
/// </summary>
class OptimizedFactorization
{
private:
    /// <summary>
    /// The non-negative integer.
    /// </summary>
    uint64_t n;

    /// <summary>
    /// The prime factors of n.
    /// </summary>
    std::shared_ptr<std::vector<PrimePower>> primeFactors;

    /// <summary>
    /// The factors of n.
    /// </summary>
    std::shared_ptr<std::vector<uint64_t>> factors;

    /// <summary>
    /// Computes the prime factors of n.
    /// </summary>
    /// <param name="verbose">Whether to output progress to std::clog.</param>
    void GeneratePrimeFactors (bool verbose);

    /// <summary>
    /// Computes the factors of n.
    /// </summary>
    void GenerateFactors ();

    /// <summary>
    /// A cache of prime numbers for trial factoring.
    /// </summary>
    static std::optional<PrimeSieve> cache;

public:
    /// <summary>
    /// Constructs an OptimizedFactorization of n.
    /// </summary>
    /// <param name="n">The number n.</param>
    OptimizedFactorization (uint64_t n);

    /// <summary>
    /// Constructs an OptimizedFactorization of n.
    /// </summary>
    /// <param name="n">The number n.</param>
    /// <param name="verbose">Whether to output progress to std::clog.</param>
    OptimizedFactorization (uint64_t n, bool verbose);

    /// <summary>
    /// Returns the list of prime factors of n.
    /// </summary>
    std::shared_ptr<const std::vector<PrimePower>> PrimeFactors () const;

    /// <summary>
    /// Returns an iterator to the beginning of the prime factors of n.
    /// </summary>
    std::vector<PrimePower>::const_iterator PrimeFactorsBegin () const;

    /// <summary>
    /// Returns an iterator to the end of the prime factors of n.
    /// </summary>
    std::vector<PrimePower>::const_iterator PrimeFactorsEnd () const;

    /// <summary>
    /// Returns the number of distinct prime factors of n.
    /// </summary>
    size_t PrimeFactorsCount () const;

    /// <summary>
    /// Returns the list of factors of n.
    /// </summary>
    std::shared_ptr<const std::vector<uint64_t>> Factors () const;

    /// <summary>
    /// Returns an iterator to the beginning of the factors of n.
    /// </summary>
    std::vector<uint64_t>::const_iterator FactorsBegin () const;

    /// <summary>
    /// Returns an iterator to the end of the prime factors of n.
    /// </summary>
    std::vector<uint64_t>::const_iterator FactorsEnd () const;

    /// <summary>
    /// Returns the number of factors of n.
    /// </summary>
    size_t FactorsCount () const;

    /// <summary>
    /// Returns the sum of the proper factors of n.
    /// </summary>
    uint64_t SumProperFactors () const;

    /// <summary>
    /// Returns whether n is prime.
    /// </summary>
    bool IsPrime () const;

    /// <summary>
    /// Returns whether n is composite.
    /// </summary>
    bool IsComposite () const;

    /// <summary>
    /// Returns whether n is perfect.
    /// </summary>
    bool IsPerfect () const;

    /// <summary>
    /// Returns whether n is deficient.
    /// </summary>
    bool IsDeficient () const;

    /// <summary>
    /// Returns whether n is abundant.
    /// </summary>
    bool IsAbundant () const;

    /// <summary>
    /// Initializes a cache of prime numbers for trial factoring.
    /// </summary>
    /// <param name="limit">The upper limit on the primes in the cache.</param>
    /// <param name="verbose">Whether to output progress to std::clog.</param>
    static void InitializeCache (size_t limit, bool verbose);
};
