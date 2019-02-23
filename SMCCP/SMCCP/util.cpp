#include "util.h"

#pragma region Log
int own_log::create()
{
	if (!file::exists(LOG_FILE_NAME))
	{
		std::ofstream o;
		o.open(LOG_FILE_NAME, std::ios::out | std::ios::app);
		o << "This is the log file from SMCCP\n--------------------------------" << std::endl;
		o.close();
		return 0;
	}
	return 1;
}

bool own_log::append(std::string whatToAppend, bool addTime)
{
	//TODO check if there could be an endless loop (when file could not be created)
	std::string alt_name = LOG_FILE_NAME;
	std::fstream f(alt_name, std::ios::app);
	if (addTime)
		whatToAppend = own_time::getTime().getString() + " - " + whatToAppend;
	if (f.good())
	{
		f << whatToAppend << std::endl;
	}
	else
	{
		own_log::create();
		append(whatToAppend, addTime);
	}
	return true;
}
#pragma endregion

#pragma region Debug
void debug::log(std::string msg)
{
#ifdef _DEBUG
	std::cout << msg << std::endl;
#endif
}

void debug::log(std::wstring msg)
{
#ifdef _DEBUG
	std::wcout << msg << std::endl;
#endif
}

void debug::pause()
{
#ifdef _DEBUG
	system("pause");
#endif // _DEBUG
}
#pragma endregion

#pragma region Time
own_time::Time own_time::getTime()
{
	std::time_t result;
	std::time(&result);
	struct tm time;
	localtime_s(&time, &result);

	own_time::Time ret_time = own_time::Time();

	ret_time.day = time.tm_mday;
	ret_time.month = time.tm_mon;
	ret_time.hour = time.tm_hour;
	ret_time.min = time.tm_min;
	ret_time.sec = time.tm_sec;
	ret_time.year = time.tm_year + 1900;

	return ret_time;
}

std::time_t own_time::getTimeStamp()
{
	return std::time(0);
}

std::string own_time::Time::getString()
{
	std::string day = this->day < 10 ? "0" + std::to_string(this->day) : std::to_string(this->day);
	std::string mon = this->month < 10 ? "0" + std::to_string(this->month + 1) : std::to_string(this->month + 1);
	std::string hour = this->hour < 10 ? "0" + std::to_string(this->hour) : std::to_string(this->hour);
	std::string min = this->min < 10 ? "0" + std::to_string(this->min) : std::to_string(this->min);
	std::string sec = this->sec < 10 ? "0" + std::to_string(this->sec) : std::to_string(this->sec);

	//INFO: time may not be correct
	return day + "." + mon + "." + std::to_string(this->year) + " " + hour + ":" + min + ":" + sec;
}
#pragma endregion

#pragma region Audio
std::map<std::string, sf::SoundBuffer> snd::sounds;

std::vector<sf::Sound> snd::players;
int snd::channels = 8;
int snd::currChannel = 0;

bool snd::LoadAllSounds()
{
	//TODO change the way audio files are loaded (index of files to be loaded)
	own_log::append("\n**********************************\nLoading audio files", false);
	bool no_error = true;

	sf::SoundBuffer bufToCopy;
	//Set up players
	for (int i = 0; i < snd::channels; i++)
		snd::players.push_back(sf::Sound());

	std::string path = SOUND_PATH;

	//loading msg_incmg_01.wav in to map
	if (!bufToCopy.loadFromFile(path + "msg_incmg_01.wav"))
	{
		no_error = false;
		own_log::append("Error loading: msg_incmg_01.wav");
	}
	sounds.emplace("incoming_01", sf::SoundBuffer(bufToCopy));

	//loading error_01.wav in to map
	if (!bufToCopy.loadFromFile(path + "error_01.wav"))
	{
		no_error = false;
		own_log::append("Error loading: error_01.wav");
	}
	sounds.emplace("error_01", sf::SoundBuffer(bufToCopy));

	//loading msg_snd_01.wav in to map
	if (!bufToCopy.loadFromFile(path + "msg_snd_01.wav"))
	{
		no_error = false;
		own_log::append("Error loading: msg_snd_01.wav");
	}
	sounds.emplace("send_01", sf::SoundBuffer(bufToCopy));

	std::string result = (no_error ? "true" : "false");
	own_log::append("Loaded " + std::to_string(sounds.size()) + " audiofiles - without error: " + result);
	own_log::append("**********************************", false);

	return no_error;
}

void snd::playSound(std::string name)
{
	//play sound on current Channel
	snd::players.at(snd::currChannel).setBuffer(sounds.at(name));
	snd::players.at(snd::currChannel).play();

	//iterate the channel
	currChannel++;
	if (currChannel == channels)
		currChannel = 0;
}
#pragma endregion

#pragma region Network
bool network::validIp(std::string ip)
{
	//Check if correct number of dots are there
	int dotCount = 0;
	for (int i = 0; i < (int)ip.length(); i++)
	{
		if (ip[i] == '.')
			dotCount++;
	}
	if (dotCount != 3)
		return false;
	//Check individual numbers
	int lastDotIndex = -1;
	for (int i = 0; i < 4; i++)
	{
		std::string currNum;
		bool dot = false;
		int j = lastDotIndex + 1;
		//loop for numbers before dots
		while (!dot && i < 3)
		{
			//Check if all chars until the next dot are numbers
			if (ip[j] != '.')
			{
				if (ip[j] >= 48 && ip[j] <= 57)
					currNum += ip[j];
				else
					return false;
			}
			else
			{
				lastDotIndex = j;
				dot = true;
			}

			j++;
		}
		//special case after last dot
		if (i == 3)
		{
			for (int h = lastDotIndex + 1; h < (int)ip.length(); h++)
			{
				if (ip[h] >= 48 && ip[h] <= 57)
					currNum += ip[h];
				else
					return false;
			}
		}

		//Check if, when the block has more than 1 digit, the first digit is 0, if yes, return
		if (currNum.length() > 1)
			if (currNum[0] == '0')
				return false;

		//Check for 4digit parts is unnessecary, because it is checked if the number is lower than 255
		//a 4digit number is always bigger than 255
		try
		{
			int bit = std::stoi(currNum);
			if (!(bit >= 0 && bit <= 255))
				return false;
		}
		catch (const std::invalid_argument&)
		{
			return false;
		}
	}

	return true;
}
#pragma endregion

#pragma region String
std::wstring str::str_to_wstr(std::string src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(src);
	return wide;
}

std::string str::wstr_to_str(std::wstring src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string narrow = converter.to_bytes(src);
	return narrow;
}

std::string str::createRandom(int length)
{
	std::string ret = "";
	for (int i = 0; i < length; i++)
		ret += key_lookup[rand() % (sizeof(key_lookup) - 1)];
	return ret;
}

std::string str::toString(int num, int length)
{
	std::stringstream ss;
	ss << std::setw(length) << std::setfill('0') << num;
	return ss.str();
}
std::vector<std::string> str::split(std::string str, char delim)
{
	std::vector<std::string> ret;
	std::istringstream iss(str);
	std::string s;
	while (std::getline(iss, s, delim))
	{
		ret.push_back(s);
	}
	return ret;
}

std::string str::concat(const std::string t)
{
	std::stringstream string_stream;
	string_stream << t;
	return string_stream.str();
}
#pragma endregion

#pragma region File
std::vector<std::string> file::getLines(nameType fileName)
{
	if (usable(fileName))
	{
		std::ifstream file(fileName);
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line))
		{
			lines.push_back(line);
		}
		return lines;
	}
	return std::vector<std::string>();
}

std::string file::getLine(nameType fileName, int lineNum)
{
	if (usable(fileName))
	{
		std::string result = "";
		std::ifstream file(fileName);
		for (int i = 0; i < lineNum; ++i) {
			std::getline(file, result);
		}
		return result;
	}
	return "";
}

bool file::appendToFile(nameType fileName, std::string content)
{
	if (usable(fileName))
	{
		std::ofstream o;
		o.open(LOG_FILE_NAME, std::ios::out | std::ios::app);
		o << content << std::endl;
		o.close();
		return true;
	}
	return false;
}

bool file::usable(nameType fileName)
{
	std::ifstream f(LOG_FILE_NAME);
	if (!f.good())
	{
		f.close();
		return false;
	}
	f.close();
	return true;
}

bool file::exists(nameType fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}
#pragma endregion
