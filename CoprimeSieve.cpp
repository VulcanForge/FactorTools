#include "CoprimeSieve.h"

#include <iostream>

CoprimeSieve::CoprimeSieve (size_t lowerLimit, size_t upperLimit, std::shared_ptr<const std::vector<uint64_t>> obstructions, bool verbose)
    : lowerLimit (lowerLimit), upperLimit (upperLimit), sieve (upperLimit - lowerLimit, true), obstructions (obstructions), coprimes (std::make_shared<std::vector<uint64_t>> ())
{
    if (verbose)
        for (uint64_t obstruction : *obstructions)
        {
            std::clog << "Striking out multiples of " << obstruction << "\n";

            uint64_t firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

            for (uint64_t multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                sieve.Reset (multiple - lowerLimit);
        }
    else
        for (uint64_t obstruction : *obstructions)
        {
            uint64_t firstMultiple = ((lowerLimit + obstruction - 1) / obstruction) * obstruction;

            for (uint64_t multiple = firstMultiple; multiple < upperLimit; multiple += obstruction)
                sieve.Reset (multiple - lowerLimit);
        }

    for (uint64_t i = lowerLimit; i < upperLimit; i++)
        if (sieve.Get (i - lowerLimit))
            coprimes->emplace_back (i);
}

std::pair<size_t, size_t> CoprimeSieve::Limits () const
{
    return std::pair<size_t, size_t> (lowerLimit, upperLimit);
}

std::shared_ptr<const std::vector<uint64_t>> CoprimeSieve::List () const
{
    return coprimes;
}

std::vector<uint64_t>::const_iterator CoprimeSieve::ListBegin () const
{
    return coprimes->cbegin ();
}

std::vector<uint64_t>::const_iterator CoprimeSieve::ListEnd () const
{
    return coprimes->cend ();
}

std::shared_ptr<const std::vector<uint64_t>> CoprimeSieve::ObstructionList () const
{
    return obstructions;
}

size_t CoprimeSieve::Count () const
{
    return coprimes->size ();
}

bool CoprimeSieve::IsCoprime (size_t n) const
{
    return sieve.Get (n);
}