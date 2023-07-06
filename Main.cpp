#include <cstdint>
#include <iostream>
#include <vector>

#include "Factorization.h"
#include "PrimeCount.h"
#include "PrimeSieve.h"

int main ()
{
    char c;

    while (true)
    {
        std::cout << "1: Sieve" << "\n"
             << "2: Factor" << "\n"
             << "3: Count" << "\n"
             << "4: Range" << "\n"
             << "5: Quit" << "\n";
        std::cin >> c;
        std::cout << "\n";

        if (c == '1')
        {
            uint64_t limit;
            std::cout << "Limit: ";
            std::cin >> limit;
            std::cout << "\n";
            PrimeSieve* sieve = new PrimeSieve (limit, true);
            auto plbegin = sieve->ListBegin ();
            uint64_t count = sieve->Count ();
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
                        std::cout << *(plbegin + i) << "\n";

                    std::cout << "\n";
                }
            }

            delete sieve;
        }
        else if (c == '2')
        {
            uint64_t n;
            std::cout << "n: ";
            std::cin >> n;
            std::cout << "\n";
            Factorization* factorization = new Factorization (n, true);

            if (factorization->IsPrime ())
                std::cout << "\n" << n << " is prime" << "\n";
            else
            {
                auto pfbegin = factorization->PrimeFactorsBegin ();
                auto pfend = factorization->PrimeFactorsEnd ();
                std::cout << "\n" << "Prime factors of " << n << "\n\n";

                for (auto i = pfbegin; i != pfend; i++)
                    std::cout << i->prime << "^" << i->power << "\n";

                auto fbegin = factorization->FactorsBegin ();
                auto fend = factorization->FactorsEnd ();
                std::cout << "\n" << "Factors of " << n << "\n\n";

                for (auto i = fbegin; i != fend; i++)
                    std::cout << *i << "\n";

                std::cout << "\n" << "Sum of proper factors of n: " << factorization->SumProperFactors () << "\n\n";

                if (factorization->IsPerfect ())
                    std::cout << n << " is perfect" << "\n\n";
                else if (factorization->IsDeficient ())
                    std::cout << n << " is deficient" << "\n\n";
                else
                    std::cout << n << " is abundant" << "\n\n";
            }

            delete factorization;
        }
        else if (c == '3')
        {
            uint64_t n;
            std::cout << "n: ";
            std::cin >> n;
            std::cout << "\n";
            PrimeSieve* sieve = new PrimeSieve (n, false);
            std::cout << "Found " << sieve->Count () << " primes less than " << n << "\n"
                 << "Legendre estimate: " << LegendreCount (n) << " primes less than " << n << "\n"
                 << "Logarithmic integral estimate: " << LiCount (n) << " primes less than " << n << "\n\n";
        }
        else if (c == '4')
        {
            uint64_t lower, upper;
            std::cout << "Lower bound: ";
            std::cin >> lower;
            std::cout << "Upper bound: ";
            std::cin >> upper;
            std::cout << "\n";

            std::shared_ptr<PrimeSieve> sieve = std::make_shared<PrimeSieve> (uint64_t (sqrt (upper)));

            for (uint64_t n = lower; n < upper; n++)
            {
                Factorization* factorization = new Factorization (n, sieve);

                if (factorization->IsPrime ())
                    std::cout << n << " is prime" << "\n\n";
                else
                {
                    auto pfbegin = factorization->PrimeFactorsBegin ();
                    auto pfend = factorization->PrimeFactorsEnd ();
                    std::cout << "Prime factors of " << n << "\n\n";

                    for (auto i = pfbegin; i != pfend; i++)
                        std::cout << i->prime << "^" << i->power << "\n";

                    auto fbegin = factorization->FactorsBegin ();
                    auto fend = factorization->FactorsEnd ();
                    std::cout << "\n" << "Factors of " << n << "\n\n";

                    for (auto i = fbegin; i != fend; i++)
                        std::cout << *i << "\n";

                    std::cout << "\n" << "Sum of proper factors of n: " << factorization->SumProperFactors () << "\n\n";

                    if (factorization->IsPerfect ())
                        std::cout << n << " is perfect" << "\n\n";
                    else if (factorization->IsDeficient ())
                        std::cout << n << " is deficient" << "\n\n";
                    else
                        std::cout << n << " is abundant" << "\n\n";
                }

                delete factorization;
            }
        }
        else if (c == '5')
            break;
        else
            std::cout << "Bad option: " << c << "\n\n";
    }

    return 0;
}
