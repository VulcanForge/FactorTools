#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>

#include <gmpxx.h>

#include <FactorTools.h>

int main ()
{
    char c;

    while (true)
    {
        std::cout
            << "1: Sieve\n"
            << "2: Factor\n"
            << "3: Count\n"
            << "4: Iterator\n"
            << "5: Factor Sieve\n"
            << "6: Test\n"
            << "7: Quit\n";
        std::cin >> c;
        std::cout << "\n";

        if (c == '1')
        {
            uint64_t limit;
            std::cout << "Limit: ";
            std::cin >> limit;
            std::cout << "\n";
            PrimeSieve sieve (limit, true);
            size_t count = sieve.Count ();
            std::cout << "Found " << count << " primes less than " << limit << "\n\n";

            while (true)
            {
                size_t index;
                std::cout << "Index (-1 to return to menu): ";
                std::cin >> index;
                std::cout << "\n";

                if (index == -1)
                    break;
                else if (index >= count)
                    std::cout << "Index too large\n\n";
                else
                {
                    for (size_t i = index; i < index + 10 && i < count; i++)
                        std::cout << sieve.Primes ()[i] << "\n";

                    std::cout << "\n";
                }
            }
        }
        else if (c == '2')
        {
            uint64_t n;
            std::cout << "n: ";
            std::cin >> n;
            std::cout << "\n";
            Factorization factorization (n, true);

            if (factorization.IsPrime ())
                std::cout << n << " is prime\n";
            else
            {
                std::cout << "Prime factors of " << n << "\n\n";

                for (PrimePower primePower : factorization.PrimeFactors ())
                    std::cout << primePower.prime << "^" << primePower.power << "\n";

                std::cout << "\nFactors of " << n << "\n\n";

                for (uint64_t factor : factorization.Factors ())
                    std::cout << factor << "\n";
            }

            std::cout
                << "\nomega(n): " << factorization.SmallOmega ()
                << "\nOmega(n): " << factorization.BigOmega ()
                << "\ntau(n): " << factorization.Tau ()
                << "\nSum of proper factors of n: " << factorization.SumProperFactors ()
                << "\nsigma1(n): " << factorization.Sigma1 ()
                << "\nmu(n): " << factorization.Mu ()
                << "\nlambda(n): " << factorization.SmallLambda ()
                << "\nRadical of n: " << factorization.Radical ()
                << "\nphi(n): " << factorization.EulerPhi ()
                << "\nCarmichael function of n: " << factorization.CarmichaelFunction ()
                << "\n\n";

            if (factorization.IsPerfect ())
                std::cout << n << " is perfect\n\n";
            else if (factorization.IsDeficient ())
                std::cout << n << " is deficient\n\n";
            else
                std::cout << n << " is abundant\n\n";
        }
        else if (c == '3')
        {
            uint64_t n;
            std::cout << "n: ";
            std::cin >> n;
            std::cout << "\n";
            PrimeSieve sieve (n, false);
            std::cout
                << "Found " << sieve.Count () << " primes less than " << n << "\n"
                << "Legendre estimate: " << LegendreCount (n) << " primes less than " << n << "\n"
                << "Logarithmic integral estimate: " << LiCount (n) << " primes less than " << n << "\n\n";
        }
        else if (c == '4')
        {
            std::cout
                << "1: Prime Sets\n"
                << "2: Prime Tuples\n"
                << "3: Factorizations\n";
            std::cin >> c;
            std::cout << "\n";

            if (c == '1')
            {
                uint64_t limit;
                std::cout << "Limit: ";
                std::cin >> limit;
                std::cout << "\n";
                BoundedPrimeSets bps (limit);
                std::cout
                    << "1 = (empty product)\n"
                    << "mu(1) = 1\n";

                for (auto bpsi = ++bps.Begin (), bpsEnd = bps.End (); bpsi != bpsEnd; bpsi++)
                {
                    std::cout << bpsi.N () << " = " << bpsi.Primes ()[0];

                    for (auto prime = ++bpsi.Primes ().cbegin (); prime != bpsi.Primes ().cend (); prime++)
                        std::cout << " * " << *prime;

                    std::cout
                        << "\n"
                        << "mu(" << bpsi.N () << ") = " << bpsi.MoebiusN () << "\n";
                }

                std::cout << "\n";
            }
            else if (c == '2')
            {
                uint64_t limit;
                std::cout << "Limit: ";
                std::cin >> limit;
                std::cout << "\n";
                uint16_t tupleSize;
                std::cout << "Tuple size: ";
                std::cin >> tupleSize;
                std::cout << "\n";

                if (tupleSize == 0)
                    std::cout << "1 = (empty product)\n\n";
                else
                {
                    BoundedPrimeFixedSizeSets bpfss (limit, tupleSize);

                    for (auto bffssi = bpfss.Begin (), bptEnd = bpfss.End (); bffssi != bptEnd; bffssi++)
                    {
                        std::cout << bffssi.N () << " = " << bffssi.Primes ()[0];

                        for (auto prime = ++bffssi.Primes ().cbegin (); prime != bffssi.Primes ().cend (); prime++)
                            std::cout << " * " << *prime;

                        std::cout
                            << "\n"
                            << "mu(" << bffssi.N () << ") = " << bffssi.MoebiusN () << "\n";
                    }

                    std::cout << "\n";
                }
            }
            else if (c == '3')
            {
                uint64_t limit;
                std::cout << "Limit: ";
                std::cin >> limit;
                std::cout << "\n";
                BoundedFactorizations bf (limit);
                std::cout << "1 = (empty product)\n";

                for (auto bfi = ++bf.Begin (), bfEnd = bf.End (); bfi != bfEnd; bfi++)
                {
                    std::cout << bfi.N () << " = " << bfi.Factorization ()[0].prime;

                    if (bfi.Factorization ()[0].power > 1)
                        std::cout << "^" << bfi.Factorization ()[0].power;

                    for (auto primePower = ++bfi.Factorization ().cbegin (); primePower != bfi.Factorization ().cend (); primePower++)
                    {
                        std::cout << " * " << primePower->prime;

                        if (primePower->power > 1)
                            std::cout << "^" << primePower->power;
                    }

                    std::cout
                        << "\n"
                        << "mu(" << bfi.N () << ") = " << bfi.MoebiusN () << "\n";
                }

                std::cout << "\n";
            }
            else
                std::cout << "Bad option " << c << "\n\n";
        }
        else if (c == '5')
        {
            uint64_t limit;
            std::cout << "Limit: ";
            std::cin >> limit;
            std::cout << "\n";
            FactorSieve sieve (limit);

            for (uint64_t n = 0; n < limit; n++)
                std::cout << n << ": " << sieve.LeastPrimeFactor (n) << "\n";

            std::cout << "\n";
        }
        else if (c == '6')
        {
            mpz_class n;
            std::cout << "n: ";
            std::cin >> n;
            mpz_class base;
            std::cout << "Base: ";
            std::cin >> base;
            std::cout << n;

            if (MillerRabinProbabilisticTest (n, base))
                std::cout << " is probably prime.\n\n";
            else
                std::cout << " is composite.\n\n";
        }
        else if (c == '7')
            break;
        else
            std::cout << "Bad option: " << c << "\n\n";
    }

    return 0;
}
