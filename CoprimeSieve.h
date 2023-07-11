#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "BitArray.h"

// An Eratosthenes-type sieve to return all numbers in a given range coprime to a given list of obstructions.
class CoprimeSieve
{
private:
    // The underlying bit array.
    BitArray sieve;

    // The inclusive lower bound on the numbers sieved.
    size_t lowerLimit;
    
    // The exclusive upper bound on the numbers sieved.
    size_t upperLimit;

    // The numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::shared_ptr<std::vector<uint64_t>> coprimes;

    // The list of obstructions.
    std::shared_ptr<const std::vector<uint64_t>> obstructions;

public:
    // Constructs a CoprimeSieve over ['lowerLimit', 'upperLimit') with the given obstructions
    // and optionally outputs progress to 'clog'.
    CoprimeSieve (size_t lowerLimit, size_t upperLimit, std::shared_ptr<const std::vector<uint64_t>> obstructions, bool verbose = false);

    // Returns the inclusive lower and the exclusive upper bounds on the numbers sieved.
    std::pair<size_t, size_t> Limits () const;

    // Returns the list of numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::shared_ptr<const std::vector<uint64_t>> List () const;

    // Returns an iterator to the beginning of the numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::vector<uint64_t>::const_iterator ListBegin () const;

    // Returns an iterator to the end of the numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    std::vector<uint64_t>::const_iterator ListEnd () const;

    // Returns the list of obstructions.
    std::shared_ptr<const std::vector<uint64_t>> ObstructionList () const;
    
    // Returns the number of numbers in ['lowerLimit', 'upperLimit') coprime to every element of 'obstructions'.
    size_t Count () const;

    // Returns whether 'n' is coprime to every element of obstructions, if n is in ['lowerLimit', 'upperLimit').
    bool IsCoprime (size_t n) const;
};
