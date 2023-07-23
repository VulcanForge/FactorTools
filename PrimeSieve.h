#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "BitArray.h"

// An Eratosthenes prime sieve.
class PrimeSieve
{
private:
    // The underlying bit array.
    BitArray sieve;

    // The exclusive upper bound on the numbers sieved.
    size_t limit;

    // The prime numbers in [0, 'limit').
    std::shared_ptr<std::vector<uint64_t>> primes;

public:
    // Constructs a PrimeSieve over [0, limit) and optionally outputs progress to 'clog'.
    PrimeSieve (size_t limit, bool verbose = false);

    // Returns the exclusive upper bound on the numbers sieved.
    size_t Limit () const;

    // Returns the list of primes in [0, 'limit').
    std::shared_ptr<const std::vector<uint64_t>> Primes () const;

    // Returns an iterator to the beginning of the primes in [0, 'limit').
    std::vector<uint64_t>::const_iterator PrimesBegin () const;

    // Returns an iterator to the end of the primes in [0, 'limit').
    std::vector<uint64_t>::const_iterator PrimesEnd () const;

    // Returns the number of primes in [0, 'limit').
    size_t Count () const;

    // Returns the number of primes in [0, 'n'], if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    size_t PrimePi (size_t n) const;

    // Returns whether 'n' is prime, if 'n' is in [0, 'limit').
    // Behaviour if 'n' >= 'limit' is undefined.
    bool IsPrime (size_t n) const;

    // BEGIN STL-COMPATIBILITY FUNCTIONS

    // Indexes 'primes'.
    const uint64_t& operator[] (size_t index) const;

    // Duplicates of 'PrimesBegin' and 'PrimesEnd' for for-each loops.

    std::vector<uint64_t>::const_iterator begin () const;

    std::vector<uint64_t>::const_iterator end () const;
};
