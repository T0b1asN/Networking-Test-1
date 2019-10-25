#include "OwnMath.h"

namespace OMath
{
	int getCommonCoprime(int a, int b)
	{
		int it = b;
		if (a < b)
			it = a;
		for (int i = 2; i < it; i++)
		{
			if (isCoprime(a, i) && isCoprime(b, i))
				return i;
		}
		return -1;
	}

	int gcd(int a, int b)
	{
		mpz_t _a;
		mpz_init(_a);
		mpz_set_si(_a, a);
		mpz_t _b;
		mpz_init(_b);
		mpz_set_si(_b, b);
		mpz_t res;
		mpz_init(res);
		mpz_gcd(res, _a, _b);
		return mpz_get_si(res);
	}

	bool isCoprime(int a, int b)
	{
		if (a <= 1 || b <= 1)
			return false;
		if (a % 2 == 0 && b % 2 == 0)
			return false;
		if (a % b == 0 || b % a == 0)
			return false;
		return (1 == gcd(a, b));
	}

	long double getCommonCoprime_ld(long double a, long double b)
	{
		long double it = b;
		if (a < b)
			it = a;
		for (long double i = 2; i < it; i++)
		{
			if (isCoprime_ld(a, i) && isCoprime_ld(b, i))
				return i;
		}
		return -1;
	}

	long double gcd_ld(long double a, long double b)
	{
		mpz_t _a;
		mpz_init(_a);
		mpz_set_d(_a, a);
		mpz_t _b;
		mpz_init(_b);
		mpz_set_d(_b, b);
		mpz_t res;
		mpz_init(res);
		mpz_gcd(res, _a, _b);
		return mpz_get_d(res);
	}

	bool isCoprime_ld(long double a, long double b)
	{
		if (a <= 1 || b <= 1)
			return false;
		if (fmodl(a, (long double) 2) < 1 && fmodl(b, (long double)2) < 1)
			return false;
		if (fmodl(a, b) < 1 || fmodl(b, a) < 1)
			return false;
		return (1 == gcd(a, b));
	}

	int rand(int min, int max)
	{
		return min + (std::rand() % (max - min + 1));
	}

	// TODO use mpir
	long double randb_l(long double min, long double max)
	{
		long double ret = -1;

		// Create and seed the random number generator
		auto gen = std::mt19937{ std::random_device{}() };
		// Create our desired distribution
		auto dist = std::uniform_real_distribution<long double>{ min, max };
		ret = dist(gen);
		return ret;
	}

	long double randb_l(int digits)
	{
		long double min = powl(10, digits - 1);
		long double max = powl(10, digits) - 1;
		return randb_l(min, max);
	}

	long double randb_l_bits(int bits)
	{
		long double ret = -1;
		auto gen = std::mt19937{ std::random_device{}() };
		// Create our desired distribution
		long double min = std::numeric_limits<long double>::min();
		long double max = std::numeric_limits<long double>::max();

		auto dist = std::uniform_real_distribution<long double>{ min, max };
		ret = fmodl(dist(gen), (long double)(1 << bits));
		return ret;
	}

	double randb(double min, double max)
	{
		double ret = -1;

		// Create and seed the random number generator
		auto gen = std::mt19937{ std::random_device{}() };
		// Create our desired distribution
		auto dist = std::uniform_real_distribution<double>{ min, max };
		ret = dist(gen);
		return ret;
	}

	double randb(int n)
	{
		double min = pow(10, n - 1);
		double max = pow(10, n) - 1;
		return randb(min, max);
	}

	void rand_mpir(mpz_t ret, long long digits)
	{
		//TODO: fill!
	}

	int countDigits(long double d)
	{
		return (int)std::log10l(d) + 1;
	}

	int countDigits(double d)
	{
		return (int)std::log10(d) + 1;
	}

	int isProbablePrime(double num)
	{
		mpz_t n;
		mpz_init(n);
		mpz_set_d(n, num);
		gmp_randstate_t state;
		gmp_randinit_mt(state);

		return mpz_probable_prime_p(n, state, PRIME_PROPAB_TEST, 0);
	}

	double nextPrime(double num)
	{
		mpz_t ret;
		mpz_init(ret);
		mpz_t n;
		mpz_init(n);
		mpz_set_d(n, num);
		gmp_randstate_t state;
		gmp_randinit_mt(state);
		mpz_next_prime_candidate(ret, n, state);
		return mpz_get_d(ret);
	}

	long double modinv_ld(long double a, long double b)
	{
		mpz_t _a;
		mpz_init(_a);
		mpz_set_d(_a, a);
		mpz_t _b;
		mpz_init(_b);
		mpz_set_d(_b, b);
		mpz_t res;
		mpz_init(res);

		int calc_res = mpz_invert(res, _a, _b);
		if (calc_res != 0)
			return mpz_get_d(res);
		else
			return -1;
	}
}

namespace mpir_math
{
	namespace
	{
		void init_state(gmp_randstate_t state)
		{
			gmp_randinit_mt(state);
			gmp_randseed_ui(state, SEED);
		}
	}

	void mpir_init(long long seed)
	{
		SEED = seed;
		init_state(STATE);
	}

	namespace test
	{
		std::string rand_test(int bits)
		{
			mpz_t res;
			mpz_init(res);
			rand(res, bits);
			return mpz_get_str(NULL, 62, res);
		}

		std::string rand_prime_test(int bits)
		{
			mpz_t res;
			mpz_init(res);
			rand_prime(res, bits);
			return mpz_get_str(NULL, 62, res);
		}

		unsigned int rand_prime_test_numbers(int bits)
		{
			mpz_t res;
			mpz_init(res);
			rand_prime(res, bits);
			return mpz_get_ui(res);
		}
	}

	void rand(mpz_t val, int bits)
	{
		mpz_init(val);
		mp_bitcnt_t _bits = bits;
		
		//gmp_randstate_t state;
		//init_state(state);

		mpz_urandomb(val, STATE, _bits);

		//gmp_randclear(state);
	}

	void rand_prime(mpz_t val, int bits)
	{
		mpz_init(val);
		rand(val, bits);

		//gmp_randstate_t state;
		//init_state(state);

		mpz_next_prime_candidate(val, val, STATE);

		//gmp_randclear(state);
	}

	bool isCoprime(mpz_t a, mpz_t b)
	{
		if (mpz_cmp_si(a, 2) < 0 || mpz_cmp_si(b, 2) < 0)
			return false;
		mpz_t temp;
		mpz_init(temp);
		mpz_mod_ui(temp, a, 2);
		if (mpz_cmp_si(temp, 0) == 0)
		{
			mpz_mod_ui(temp, b, 2);
			if (mpz_cmp_si(temp, 0) == 0)
				return false;
		}
		mpz_mod(temp, a, b);
		if (mpz_cmp_si(temp, 0) == 0)
			return false;
		mpz_mod(temp, b, a);
		if (mpz_cmp_si(temp, 0) == 0)
			return false;
		mpz_gcd(temp, a, b);
		return (0 == mpz_cmp_si(temp, 1));
	}

	bool getCommonCoprime(mpz_t res, mpz_t a, mpz_t b)
	{
		mpz_t it;
		mpz_init(it);
		mpz_set(it, b);
		if (a < b)
			mpz_set(it, a);

		mpz_t i;
		mpz_init(i);
		mpz_set_ui(i, 2);
		while (mpz_cmp(i, it) < 0)
		{
			if (isCoprime(a, i) && isCoprime(b, i))
			{
				mpz_set(res, i);
				return true;
			}
			mpz_add_ui(i, i, 1);
		}
		return false;
	}
}

namespace mpir_helper
{
	std::string str(mpz_t n)
	{
		return mpz_get_str(NULL, 62, n);
	}

	void fill(mpz_t n, std::string str, int base)
	{
		mpz_init(n);
		mpz_set_str(n, str.c_str(), base);
	}
}
