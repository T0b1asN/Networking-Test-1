#include "RSA.h"
namespace RSA
{
	Key GenerateKey(int bitCount, bool calcE)
	{
		debug::log("starting keygeneration with primes...");
		mpz_t p;
		mpz_t q;
		mpz_init(p);
		mpz_init(q);
		mpir_math::rand_prime(p, bitCount / 2);
		mpir_math::rand_prime(q, bitCount / 2);
		debug::log("calculated primes, continuing with N...");

		Key key = Key();
		key.pubKey = PublicKey();
		key.privKey = PrivateKey();
		key.err = false;

		mpz_init(key.pubKey.N);
		mpz_init(key.privKey.N);
		mpz_init(key.pubKey.e);
		mpz_init(key.privKey.d);

		mpz_t N;
		mpz_init(N);
		mpz_mul(N, p, q);
		mpz_set(key.pubKey.N, N);
		mpz_set(key.privKey.N, N);
		debug::log("calculated N, continuing with r...");

#pragma region r
		mpz_t one;
		mpz_init(one);
		mpz_set_si(one, 1);

		mpz_t p1;
		mpz_init(p1);
		mpz_sub(p1, p, one);

		mpz_t q1;
		mpz_init(q1);
		mpz_sub(q1, q, one);

		mpz_t r;
		mpz_init(r);
		mpz_mul(r, p1, q1);
#pragma endregion

		debug::log("calculated r, continuing with e...");
		if (calcE)
		{
			if (!mpir_math::getCommonCoprime(key.pubKey.e, N, r))
				key.err = true;
		}
		else
			mpz_set_si(key.pubKey.e, 65537);

		debug::log("calculated e, continuing with d...");
		int calc_res = mpz_invert(key.privKey.d, key.pubKey.e, r);
		if (calc_res == 0)
			key.err = true;
		debug::log("finished keygeneration...");

		return key;
	}

	std::string __encrypt(std::string msg, PublicKey key)
	{
		//! leading 1 is important!
		std::string str_num = "1";
		// convert string to ascii number
		// format
		//   A   B   C   D
		// 1 065 066 067 068
		// (explanation RSA-Ideas.md)
		for (int i = 0; i < (int)msg.length(); i++)
		{
			str_num += str::toString(int(msg[i]), 3);
		}
		// set num from string
		mpz_t num;
		mpz_init(num);
		mpz_set_str(num, str_num.c_str(), 10);
		
		mpz_t res;
		mpz_init(res);
		//mpz_powm(rop, base, exp, mod)
		// (base^exp) % mod
		// base = num
		// exp = e
		// mod = N
		mpz_powm(res, num, key.e, key.N);
		
		return mpz_get_str(NULL, ENC_BASE, res);
	}

	std::string Encrypt(std::string msg, PublicKey key, int partLength)
	{
		std::string res = "";
		debug::log("Encrypt msg length " + std::to_string(msg.length()));
		for (int i = 0; i < (int)msg.length(); i+=partLength)
		{
			res += __encrypt(msg.substr(i, partLength), key);
			debug::log("i: " + std::to_string(i) + "; partlength: " + std::to_string(partLength));
			res += " ";
		}
		return res;
	}

	std::string __decrypt(std::string msg, PrivateKey key)
	{
		mpz_t crypt;
		mpz_init(crypt);
		mpz_set_str(crypt, msg.c_str(), ENC_BASE);

		mpz_t res;
		mpz_init(res);
		// mpz_powm(rop, base, exp, mod)
		// (base^exp) % mod
		// base = crypt
		// exp = d
		// mod = N
		mpz_powm(res, crypt, key.d, key.N);

		std::string res_str = "";
		//? Maybe Error here (wrong base)
		res_str = mpz_get_str(NULL, 10, res);
		
		std::string decode = "";
		for (int i = 1; i < (int)res_str.length(); i+=3)
		{
			std::string num_str = "";
			num_str += res_str[i];
			num_str += res_str[i + 1];
			num_str += res_str[i + 2];
			decode += char(std::stoi(num_str));
		}
		return decode;
	}

	std::string Decrypt(std::string msg, PrivateKey key)
	{
		std::string res = "";
		for each (std::string s in str::split(msg, ' '))
		{
			res += __decrypt(s, key);
		}
		return res;
	}

	std::string keyToStr(Key key)
	{
		std::string ret = "";
		ret += "private (d): " + mpir_helper::str(key.privKey.d) + "\n";
		ret += "private/public (N): " + mpir_helper::str(key.privKey.N) + "\n";
		ret += "public (e): " + mpir_helper::str(key.pubKey.e);
		return ret;
	}

	std::string keyToStr(PublicKey key)
	{
		std::string ret = "";
		ret += "N: " + mpir_helper::str(key.N) + "\n";
		ret += "e: " + mpir_helper::str(key.e);
		return ret;
	}

	std::string keyToStr(PrivateKey key)
	{
		std::string ret = "";
		ret += "d: " + mpir_helper::str(key.d) + "\n";
		ret += "N: " + mpir_helper::str(key.N);
		return ret;
	}
}
