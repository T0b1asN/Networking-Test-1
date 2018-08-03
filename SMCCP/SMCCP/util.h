#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <locale>
#include <codecvt>

#include <map>
#include <VECTOR>

#include <ctime>
#include <time.h>

#include "SFML.h"


#ifdef _DEBUG
#define LOG_FILE_NAME "DEBUG_LOG.txt"
#else
#define LOG_FILE_NAME "LOG.txt"
#endif
//INFO: Log file doesn't work with multiple exe's opened from the same origin at the same time
namespace own_log
{
	//Creates a log file
	//returns an error code
	//Codes:
	//	0 = Created Log
	//	1 = Log file already exists
	//	2 = different error
	int create();

	//Append string to log (new line)
	//second parameter: Should the time be added?
	//returns true if successful
	bool append(std::string whatToAppend, bool addTime = true);

	//TODO: Functions to read the log
}

namespace debug
{
	void log(std::string msg);
	void log(std::wstring msg);

	void pause();
}

namespace own_time
{
	std::string getTime();
	std::time_t getTimeStamp();
}

#define SOUND_PATH "res\\sounds\\"
namespace snd
{
	extern std::map<std::string, sf::SoundBuffer> sounds;

	extern std::vector<sf::Sound> players;
	extern int channels;
	extern int currChannel;

	//TODO Load all sounds with help of file containing all names
	bool LoadAllSounds();

	void playSound(std::string name);

}

#define DELIM "."
namespace network
{
	//Doesn't protect against 001.123.123.123 (0 as first digit of block)
	//TODO: protect against 0 as first digit of multichar block
	bool validIp(std::string ip);
}

namespace str
{
	std::wstring str_to_wstr(std::string src);
	std::string wstr_to_str(std::wstring src);
}

namespace file
{
	typedef const std::string& nameType;
	std::vector<std::string> getLines(nameType fileName);
	//counting from one
	std::string getLine(nameType fileName, int lineNum);

	//returns true if successful
	bool appendToFile(nameType fileName, std::string content);

	bool usable(nameType fileName);
	bool exists(nameType fileName);
}
