#pragma once
#include <iostream>
#include <string>
#include <ctype.h>

#define DELIM "."

namespace ntwrk
{
	//Doesn't protect against 001.123.123.123 (0 as first digit of block)
	//TODO: protect against 0 as first digit of multichar block
	bool validIp(std::string ip);

}
