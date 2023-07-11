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

    // Returns the list of primes in [0, Limit ()).
    std::shared_ptr<const std::vector<uint64_t>> List () const;

    // Returns an iterator to the beginning of the primes in [0, Limit ()).
    std::vector<uint64_t>::const_iterator ListBegin () const;

    // Returns an iterator to the end of the primes in [0, Limit ()).
    std::vector<uint64_t>::const_iterator ListEnd () const;

    // Returns the number of primes in [0, Limit ()).
    size_t Count () const;

    // Returns the number of primes in [0, 'n'], if 'n' is in [0, Limit ()).
    // Behaviour if 'n' >= Limit () is undefined.
    size_t PrimePi (size_t n) const;

    // Returns whether 'n' is prime, if 'n' is in [0, Limit ()).
    bool IsPrime (size_t n) const;
};
