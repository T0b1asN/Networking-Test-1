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

	std::string keyToStr(Key);
	std::string keyToStr(PublicKey);
	std::string keyToStr(PrivateKey);

	Key GenerateKey(int bitCount = 1024, bool calcE = false);

	//TODO make needed function inaccesible or rename them
	//text is encrypted as a whole
	std::string __encrypt(std::string msg, PublicKey key);
	//text is encrypted in chunks of size [partLength]
	std::string Encrypt(std::string msg, PublicKey key, int partLength = 4);

	//text is decrypted as a whole
	std::string __decrypt(std::string msg, PrivateKey key);
	//text is dexrypted in chunks separated by a space
	std::string Decrypt(std::string msg, PrivateKey key);
}

