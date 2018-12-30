#pragma once
#include <numeric>
#include <random>

#include "mpir.h"


namespace OMath
{
	constexpr auto PRIME_PROPAB_TEST = 10;

	int getCommonCoprime(int a, int b);
	int gcd(int a, int b);

	//TODO: Template!
	bool isCoprime(int a, int b);

	long double getCommonCoprime_ld(long double a, long double b);
	long double gcd_ld(long double a, long double b);

	//TODO: Template!
	bool isCoprime_ld(long double a, long double b);

	//https://www.exploringbinary.com/how-to-install-and-run-gmp-on-windows-using-mpir/
	int rand(int min, int max);

	long double randb_l(long double min, long double max);
	long double randb_l(int digits);
	long double randb_l_bits(int bits);

	double randb(double min, double max);
	double randb(int n);


	void rand_mpir(mpz_t ret, long long digits);

	int countDigits(long double d);
	int countDigits(double d);

	int isProbablePrime(double num);
	double nextPrime(double num);

	long double modinv_ld(long double a, long double b);
}

namespace mpir_math
{
	namespace
	{
		constexpr auto PRIME_PROPAB = 100;
		long long SEED = 0;
		gmp_randstate_t STATE;

		void init_state(gmp_randstate_t state);
	}

	void mpir_init(long long seed);

	namespace test
	{
		std::string rand_test(int bits);
		std::string rand_prime_test(int bits);
		unsigned int rand_prime_test_numbers(int bits);
	}

	void rand(mpz_t val, int bits);
	// find faster algorithm
	void rand_prime(mpz_t val, int bits);

	bool isCoprime(mpz_t a, mpz_t b);
	bool getCommonCoprime(mpz_t res, mpz_t a, mpz_t b);
}

namespace mpir_helper
{
	std::string str(mpz_t n);
	void fill(mpz_t n, std::string str);
}
