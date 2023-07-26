#include "BoundedFactorizations.h"

#include "Exponent.h"

BF::BoundedFactorizations (uint64_t upperBound)
    : bps (upperBound) {}

BFI::BoundedFactorizationIterator (const BF& parent, const BPSI& bpsi)
    : parent (parent), bpsi (bpsi) {}

std::vector<PrimePower>::const_iterator BFI::FactorizationBegin () const
{
    return factorization.cbegin ();
}

std::vector<PrimePower>::const_iterator BFI::FactorizationEnd () const
{
    return factorization.cend ();
}

uint64_t BFI::N () const
{
    return n;
}

BFI& BFI::operator++ ()
{
    // Algorithm: Attempt to increment the power of the last prime in the factorization.
    // If that is too large, repeatedly reset that power to 1 and attempt to increment the power of the previous prime.
    // If all exponent tuples for the current prime set have been exhausted, "increment" the prime set.

    // The index of the prime power to attempt to increment.
    size_t index = factorization.size () - 1;

    // Attempt to increment the prime power at 'index' (and end the loop).
    while (index < factorization.size () && n * factorization[index].prime >= parent.bps.upperBound)
    {
        n /= Pow (factorization[index].prime, factorization[index].power - 1);
        factorization[index].power = 1;
        // If 'index' is 0, it will become UINT64_MAX and be > 'factorization.size ()'.
        // Then the loop will terminate,
        // the next conditional will recognize that all exponent tuples have been exhausted,
        // and the prime set itself will "increment".
        index--;
    }
    
    // Actually increment the prime power at 'index', unless all exponent tuples for the current prime set have been exhausted.
    if (index < factorization.size ())
    {
        n *= factorization[index].prime;
        factorization[index].power++;
        return *this;
    }

    // "Increment" the prime set and reset all exponents to 1.
    bpsi++;
    n = bpsi.N ();
    factorization.clear ();
    auto primesEnd = bpsi.PrimesEnd ();

    for (auto prime = bpsi.PrimesBegin (); prime != primesEnd; prime++)
        factorization.emplace_back (*prime, 1);

    return *this;
}

BFI BFI::operator++ (int)
{
    BFI old = *this;
    operator++ ();
    return old;
}

bool operator== (const BFI& left, const BFI& right)
{
    return left.n == right.n;
}

BFI BF::Begin () const
{
    BFI begin (*this, bps.Begin ());
    begin.n = 1;
    // 'factorization' starts empty as 'n' = 1.
    return begin;
}

BFI BF::End () const
{
    BFI end (*this, bps.End ());
    // There is no need to initialize 'factorization' as it is not considered in iterator comparison.
    end.n = 0;
    return end;
}
