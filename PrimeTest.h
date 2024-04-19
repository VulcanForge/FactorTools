#pragma once

#include <cstdint>

#include <gmp.h>

bool FermatProbabilisticTest (const mpz_t n, const mpz_t base);

bool MillerRabinProbabilisticTest (const mpz_t n, const mpz_t base);
