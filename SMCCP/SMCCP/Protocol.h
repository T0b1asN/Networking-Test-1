#pragma once
#include <string>

namespace prot
{
	// Send a message (followed by message)
	constexpr auto msg = "MSG";
	// Send RSA-key
	constexpr auto rsa_key = "RSA";
	// notify about missing arguments in sent command
	constexpr auto missing_args = "MISSING_ARGS";
	//general error
	constexpr auto error = "ERROR";

	namespace c
	{
		// send the name to the server
		constexpr auto name = "NAME";
	}

	namespace s
	{
		// send a message without sound
		constexpr auto msg_nosound = "MSGNS";
		// sed the response to the name packet
		constexpr auto name_resp = "NAME_RESP";
		// notify about shutdown
		constexpr auto shutdown = "SHUTDOWN";
		// send reason for forced disconnect
		constexpr auto disconnect = "MSGDC";
		// expected another command at this point
		constexpr auto wrong_cmd_err = "WRONG_CMD";
		// should have sended RSA-key
		constexpr auto missing_rsa_err = "RSA_MISSING";
		// should have sended name
		constexpr auto missing_name_err = "NAME_MISSING";
	}

	namespace rsa
	{
		//global value for bitcount of key
		constexpr auto key_bitcnt = 512;
		//how big the chunks for encryption should be
		constexpr auto chunkSize = 8;
	}

	//removes the token and the space at the start of the function
	//(only removes token.length()+1 chars from beginning of string)
	inline std::string remToken(std::string from, std::string token)
	{
		//TODO make better
		return from.erase(0, token.length() + 1);
	}
}
