#include "PrimeTest.h"

bool FermatProbabilisticTest (const mpz_t n, const mpz_t base)
{
    mpz_t exponent;
    mpz_init (exponent);
    mpz_sub_ui (exponent, n, 1);
    mpz_t power;
    mpz_init (power);
    mpz_powm (power, base, exponent, n);
    bool result = (mpz_cmp_ui (power, 1) == 0);
    mpz_clear (exponent);
    mpz_clear (power);
    return result;
}

bool MillerRabinProbabilisticTest (const mpz_t n, const mpz_t base)
{
    mpz_t oddPartExponent;
    mpz_init (oddPartExponent);
    mpz_sub_ui (oddPartExponent, n, 1);
    mpz_t two;
    mpz_init_set_ui (two, 2);
    mp_bitcnt_t twoAdicValuationExponent = mpz_remove (oddPartExponent, oddPartExponent, two);
    mpz_t runningPower;
    mpz_init (runningPower);
    mpz_powm (runningPower, base, oddPartExponent, n);

    if (mpz_cmp_ui (runningPower, 1) == 0)
    {
        mpz_clear (oddPartExponent);
        mpz_clear (two);
        mpz_clear (runningPower);
        return true;
    }

    mpz_t n_minus_one;
    mpz_init (n_minus_one);
    mpz_sub_ui (n_minus_one, n, 1);

    if (mpz_cmp (runningPower, n_minus_one) == 0)
    {
        mpz_clear (oddPartExponent);
        mpz_clear (two);
        mpz_clear (runningPower);
        mpz_clear (n_minus_one);
        return true;
    }

    for (mp_bitcnt_t r = 1; r < twoAdicValuationExponent; r++)
    {
        mpz_mul (runningPower, runningPower, runningPower);
        mpz_mod (runningPower, runningPower, n);

        if (mpz_cmp (runningPower, n_minus_one) == 0)
        {
            mpz_clear (oddPartExponent);
            mpz_clear (two);
            mpz_clear (runningPower);
            mpz_clear (n_minus_one);
            return true;
        }
    }

    mpz_clear (oddPartExponent);
    mpz_clear (two);
    mpz_clear (runningPower);
    mpz_clear (n_minus_one);
    return false;
}
