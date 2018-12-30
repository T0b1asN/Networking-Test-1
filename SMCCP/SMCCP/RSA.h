#pragma once
#include "OwnMath.h"
#include "util.h"

#include <vector>

namespace RSA
{
	constexpr auto ENC_BASE = 62;

	struct PublicKey
	{
		mpz_t N;
		mpz_t e;
	};

	struct PrivateKey
	{
		mpz_t N;
		mpz_t d;
	};

	struct Key
	{
		PublicKey pubKey;
		PrivateKey privKey;
		bool err;
	};


	Key GenerateKey(int bitCount = 1024, bool calcE = false);

	//TODO better naming for long text variants
	std::string Encrypt(std::string msg, PublicKey key);
	std::string Encrypt_Long(std::string msg, PublicKey key, int partLength = 4);

	std::string Decrypt(std::string msg, PrivateKey key);
	std::string Decrypt_Long(std::string msg, PrivateKey key);
}

