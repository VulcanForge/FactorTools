#pragma once

#include <cstdint>

#include <gmpxx.h>

bool FermatProbabilisticTest (const mpz_class n, const mpz_class base);

bool MillerRabinProbabilisticTest (const mpz_class n, const mpz_class base);
