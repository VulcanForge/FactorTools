#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "BitArray.h"

/// <summary>
/// An Eratosthenes prime sieve.
/// </summary>
class PrimeSieve
{
private:
    /// <summary>
    /// The underlying bit array.
    /// </summary>
    BitArray sieve;

    /// <summary>
    /// The exclusive upper bound on the numbers sieved.
    /// </summary>
    size_t limit;

    /// <summary>
    /// The prime numbers in [0, limit).
    /// </summary>
    std::shared_ptr<std::vector<uint64_t>> primes;

public:
    /// <summary>
    /// Constructs a PrimeSieve over [0, limit).
    /// </summary>
    /// <param name="limit">The exclusive upper bound on the numbers sieved.</param>
    PrimeSieve (size_t limit);

    /// <summary>
    /// Constructs a PrimeSieve over [0, limit).
    /// </summary>
    /// <param name="limit">The exclusive upper bound on the numbers sieved.</param>
    /// <param name="verbose">Whether to output progress to std::clog.</param>
    PrimeSieve (size_t limit, bool verbose);

    /// <summary>
    /// Returns the exclusive upper bound on the numbers sieved.
    /// </summary>
    size_t Limit () const;

    /// <summary>
    /// Returns the list of primes in [0, Limit ()).
    /// </summary>
    std::shared_ptr<const std::vector<uint64_t>> List () const;

    /// <summary>
    /// Returns an iterator to the beginning of the primes in [0, Limit ()).
    /// </summary>
    std::vector<uint64_t>::const_iterator ListBegin () const;

    /// <summary>
    /// Returns an iterator to the end of the primes in [0, Limit ()).
    /// </summary>
    std::vector<uint64_t>::const_iterator ListEnd () const;

    /// <summary>
    /// Returns the number of primes in [0, Limit ()).
    /// </summary>
    size_t Count () const;

    /// <summary>
    /// Returns whether n is prime, if n is in [0, Limit ()).
    /// </summary>
    /// <param name="n">The number n.</param>
    bool IsPrime (size_t n) const;
};
