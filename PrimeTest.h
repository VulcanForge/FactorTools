#pragma once

#include <gmpxx.h>

// Runs a Fermat probabilistic prime test on 'n' using the given base.
// 'base' should not be a multiple of 'n'.
// If 'n' is not a Carmichael number (a set of asymptotic density 0) and composite,
// at least 50% of bases detect its compositeness.
bool FermatProbabilisticTest (const mpz_class n, const mpz_class base);

// Runs a Miller-Rabin probabilistic prime test on 'n' using the given base.
// 'base' should not be a multiple of 'n', and 'n' should be odd.
// If 'n' is composite, at least 75% of bases detect its compositeness.
bool MillerRabinProbabilisticTest (const mpz_class n, const mpz_class base);
