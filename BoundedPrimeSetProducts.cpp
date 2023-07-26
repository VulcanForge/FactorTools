#include "BoundedPrimeSetProducts.h"

#include "Exponent.h"

BPSP::BoundedPrimeSetProducts (const BPSI& bpsi)
    : bpsi (bpsi) {}

BPSPI::BoundedPrimeSetProductIterator (const BPSP& parent)
    : parent (parent) {}

std::vector<PrimePower>::const_iterator BPSPI::FactorizationBegin () const
{
    return factorization.cbegin ();
}

std::vector<PrimePower>::const_iterator BPSPI::FactorizationEnd () const
{
    return factorization.cend ();
}

uint64_t BPSPI::N () const
{
    return n;
}

BPSPI& BPSPI::operator++ ()
{
    // Algorithm: Attempt to increment the power of the last prime in the factorization.
    // If that is too large, repeatedly reset that power to 1 and attempt to increment the power of the previous prime.
    // The index of the prime power to attempt to increment.
    size_t index = factorization.size () - 1;

    // Attempt to increment the prime power at 'index' (and end the loop).
    while (index < factorization.size () && n * factorization[index].prime >= parent.bpsi.parent.upperBound)
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

    // All exponent tuples for the current prime set have been exhausted.
    // Move into the end iterator state.
    n = 0;
    return *this;
}

BPSPI BPSPI::operator++ (int)
{
    BPSPI old = *this;
    operator++ ();
    return old;
}

bool operator== (const BPSPI& left, const BPSPI& right)
{
    return left.n == right.n;
}

BPSPI BPSP::Begin () const
{
    BPSPI begin (*this);
    begin.n = 1;
    auto primesEnd = bpsi.PrimesEnd ();

    // Initialize all exponents to 1.
    for (auto prime = bpsi.PrimesBegin (); prime != primesEnd; prime++)
    {
        begin.factorization.emplace_back (*prime, 1);
        begin.n *= begin.factorization.back ().prime;
    }

    return begin;
}

BPSPI BPSP::End () const
{
    BPSPI end (*this);
    end.n = 0;
    // There is no need to initialize 'factorization' as it is not considered in iterator comparison.
    return end;
}
