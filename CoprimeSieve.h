#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "BitArray.h"

/// <summary>
/// A sieve to return all numbers in a given range coprime to a given list of obstructions.
/// </summary>
class CoprimeSieve
{
private:
    /// <summary>
    /// The underlying bit array.
    /// </summary>
    BitArray sieve;

    /// <summary>
    /// The inclusive lower bound on the numbers sieved.
    /// </summary>
    size_t lowerLimit;
    
    /// <summary>
    /// The exclusive upper bound on the numbers sieved.
    /// </summary>
    size_t upperLimit;

    /// <summary>
    /// The numbers in [lowerLimit, upperLimit) coprime to every element of obstructions.
    /// </summary>
    std::shared_ptr<std::vector<uint64_t>> coprimes;

    /// <summary>
    /// The list of obstructions.
    /// </summary>
    std::shared_ptr<const std::vector<uint64_t>> obstructions;

public:
    /// <summary>
    /// Constructs a CoprimeSieve over [lowerLimit, upperLimit) with the given obstructions.
    /// </summary>
    /// <param name="lowerLimit">The inclusive lower bound on the numbers sieved.</param>
    /// <param name="upperLimit">The exclusive upper bound on the numbers sieved.</param>
    /// <param name="obstructions">The list of obstructions.</param>
    CoprimeSieve (size_t lowerLimit, size_t upperLimit, std::shared_ptr<const std::vector<uint64_t>> obstructions);

    /// <summary>
    /// Constructs a CoprimeSieve over [lowerLimit, upperLimit) with the given obstructions.
    /// </summary>
    /// <param name="lowerLimit">The exclusive lower bound on the numbers sieved.</param>
    /// <param name="upperLimit">The inclusive upper bound on the numbers sieved.</param>
    /// <param name="obstructions">The list of obstructions.</param>
    /// <param name="verbose">Whether to output progress to std::clog.</param>
    CoprimeSieve (size_t lowerLimit, size_t upperLimit, std::shared_ptr<const std::vector<uint64_t>> obstructions, bool verbose);

    /// <summary>
    /// Returns the inclusive lower and the exclusive upper bounds on the numbers sieved.
    /// </summary>
    std::pair<size_t, size_t> Limits () const;

    /// <summary>
    /// Returns the list of numbers in [lowerLimit, upperLimit) coprime to every element of obstructions.
    /// </summary>
    std::shared_ptr<const std::vector<uint64_t>> List () const;

    /// <summary>
    /// Returns an iterator to the beginning of the numbers in [lowerLimit, upperLimit) coprime to every element of obstructions.
    /// </summary>
    std::vector<uint64_t>::const_iterator ListBegin () const;

    /// <summary>
    /// Returns an iterator to the end of the numbers in [lowerLimit, upperLimit) coprime to every element of obstructions.
    /// </summary>
    std::vector<uint64_t>::const_iterator ListEnd () const;

    /// <summary>
    /// Returns the list of obstructions.
    /// </summary>
    std::shared_ptr<const std::vector<uint64_t>> ObstructionList () const;
    
    /// <summary>
    /// Returns the number of numbers in [lowerLimit, upperLimit) coprime to every element of obstructions.
    /// </summary>
    size_t Count () const;

    /// <summary>
    /// Returns whether n is coprime to every element of obstructions, if n is in [lowerLimit, upperLimit).
    /// </summary>
    /// <param name="n">The number n.</param>
    bool IsCoprime (size_t n) const;
};
