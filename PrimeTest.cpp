#include <PrimeTest.h>

bool FermatProbabilisticTest (const mpz_class n, const mpz_class base)
{
    // Standard Fermat test algorithm.
    mpz_class exponent = n - 1;
    mpz_class power;
    mpz_powm (power.get_mpz_t (), base.get_mpz_t (), exponent.get_mpz_t (), n.get_mpz_t ());
    return power == 1;
}

bool MillerRabinProbabilisticTest (const mpz_class n, const mpz_class base)
{
    // Standard Miller-Rabin test algorithm.
    mpz_class oddPartExponent = n - 1;
    mpz_class two = 2;
    mp_bitcnt_t twoAdicValuationExponent = mpz_remove
    (
        oddPartExponent.get_mpz_t (),
        oddPartExponent.get_mpz_t (),
        two.get_mpz_t ()
    );
    mpz_class runningPower;
    mpz_powm (runningPower.get_mpz_t (), base.get_mpz_t (), oddPartExponent.get_mpz_t (), n.get_mpz_t ());

    if (runningPower == 1 || runningPower == n - 1)
        return true;

    for (mp_bitcnt_t r = 1; r < twoAdicValuationExponent; ++r)
    {
        runningPower = (runningPower * runningPower) % n;

        if (runningPower == n - 1)
            return true;
        else if (runningPower == 1)
            // -1 has not been encountered previously, so the previous power was a non-trivial square root of 1.
            return false;
    }

    return false;
}
