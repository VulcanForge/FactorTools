#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>

#include "BitArray.h"
#include "BoundedFactorizations.h"
#include "BoundedPrimeSetProducts.h"
#include "BoundedPrimeSets.h"
#include "CoprimeSieve.h"
#include "Exponent.h"
#include "Factorization.h"
#include "PrimeCount.h"
#include "PrimePower.h"
#include "PrimeSieve.h"

int main ()
{
    char c;

    while (true)
    {
        std::cout
            << "1: Sieve" << "\n"
            << "2: Factor" << "\n"
            << "3: Count" << "\n"
            << "4: Iterator" << "\n"
            << "5: Quit" << "\n";
        std::cin >> c;
        std::cout << "\n";

        if (c == '1')
        {
            uint32_t limit;
            std::cout << "Limit: ";
            std::cin >> limit;
            std::cout << "\n";
            PrimeSieve<uint32_t> sieve (limit, true);
            size_t count = sieve.Count ();
            std::cout << "\n" << "Found " << count << " primes less than " << limit << "\n\n";

            while (true)
            {
                size_t index;
                std::cout << "Index (-1 to return to menu): ";
                std::cin >> index;
                std::cout << "\n";

                if (index == -1)
                    break;
                else if (index >= count)
                    std::cout << "Index too large" << "\n\n";
                else
                {
                    for (size_t i = index; i < index + 10 && i < count; i++)
                        std::cout << sieve[i] << "\n";

                    std::cout << "\n";
                }
            }
        }
        else if (c == '2')
        {
            uint32_t n;
            std::cout << "n: ";
            std::cin >> n;
            std::cout << "\n";
            Factorization<uint32_t> factorization (n, true);

            if (factorization.IsPrime ())
                std::cout << "\n" << n << " is prime" << "\n";
            else
            {
                std::cout << "\n" << "Prime factors of " << n << "\n\n";

                for (PrimePower primePower : *(factorization.PrimeFactors ()))
                    std::cout << primePower.prime << "^" << primePower.power << "\n";

                std::cout << "\n" << "Factors of " << n << "\n\n";

                for (uint32_t factor : *(factorization.Factors ()))
                    std::cout << factor << "\n";

                std::cout << "\n" << "Sum of proper factors of n: " << factorization.SumProperFactors () << "\n\n";

                if (factorization.IsPerfect ())
                    std::cout << n << " is perfect" << "\n\n";
                else if (factorization.IsDeficient ())
                    std::cout << n << " is deficient" << "\n\n";
                else
                    std::cout << n << " is abundant" << "\n\n";
            }
        }
        else if (c == '3')
        {
            uint32_t n;
            std::cout << "n: ";
            std::cin >> n;
            std::cout << "\n";
            PrimeSieve<uint32_t> sieve (n, false);
            std::cout
                << "Found " << sieve.Count () << " primes less than " << n << "\n"
                << "Legendre estimate: " << LegendreCount (n) << " primes less than " << n << "\n"
                << "Logarithmic integral estimate: " << LiCount (n) << " primes less than " << n << "\n\n";
        }
        else if (c == '4')
        {
            uint32_t limit;
            std::cout << "Limit: ";
            std::cin >> limit;
            std::cout << "\n";

            BoundedPrimeSets<uint32_t> bps (limit);

            for (auto bpsi = bps.Begin (), bpsEnd = bps.End (); bpsi != bpsEnd; bpsi++)
            {
                std::cout << "Primes: ";

                if (bpsi.N () == 1)
                {
                    std::cout
                        << "(none)\n"
                        << "\t1\n";
                    continue;
                }

                std::cout << *(bpsi.PrimesBegin ());

                for (auto prime = ++bpsi.PrimesBegin (), primesEnd = bpsi.PrimesEnd (); prime != primesEnd; prime++)
                    std::cout << ", " << *prime;

                std::cout << "\n";

                BoundedPrimeSetProducts<uint32_t> bpsp (bpsi);

                for (auto bpspi = bpsp.Begin (), bpspEnd = bpsp.End (); bpspi != bpspEnd; bpspi++)
                    std::cout << "\t" << bpspi.N () << "\n";
            }
        }
        else if (c == '5')
            break;
        else
            std::cout << "Bad option: " << c << "\n\n";
    }

    return 0;
}
