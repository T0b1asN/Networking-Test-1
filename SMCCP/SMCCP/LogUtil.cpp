#include "LogUtil.h"

int own_log::CreateLog()
{
	std::ofstream o;
	std::ifstream f(LOG_FILE_NAME);
	if (!f.good())
	{
		f.close();
		o.open(LOG_FILE_NAME, std::ios::out | std::ios::app);
		o << "This is the log file from SMCCP\n--------------------------------" << std::endl;
		o.close();
	}
	else
	{
		f.close();
		return 1;
	}

	return 0;
}

bool own_log::AppendToLog(std::string whatToAppend, bool addTime)
{
	//TODO check if there could be an endless loop (when file could not be created
	std::string alt_name = LOG_FILE_NAME;
	std::fstream f(alt_name, std::ios::app);
	if (addTime)
		whatToAppend = getTime() + " - " + whatToAppend;
	if (f.good())
	{
		f << whatToAppend << std::endl;
	}
	else
	{
		own_log::CreateLog();
		AppendToLog(whatToAppend, addTime);
	}
	return true;
}

void own_log::pushMsgToCommandIfDebug(std::string msg)
{
#ifdef _DEBUG
	std::cout << msg << std::endl;
#endif
}

std::string own_log::getTime()
{
	std::time_t result;
	std::time(&result);
	struct tm time;
	localtime_s(&time, &result);

	std::string day = time.tm_mday < 10 ? "0" + std::to_string(time.tm_mday) : std::to_string(time.tm_mday);
	std::string mon = time.tm_mon < 10 ? "0" + std::to_string(time.tm_mon + 1) : std::to_string(time.tm_mon + 1);
	std::string hour = time.tm_hour < 10 ? "0" + std::to_string(time.tm_hour) : std::to_string(time.tm_hour);
	std::string min = time.tm_min < 10 ? "0" + std::to_string(time.tm_min) : std::to_string(time.tm_min);
	std::string sec = time.tm_sec < 10 ? "0" + std::to_string(time.tm_sec) : std::to_string(time.tm_sec);

	//INFO: time may not be correct
	return day + "." + mon + "." + std::to_string(time.tm_year + 1900) + " " + hour + ":" + min + ":" + sec;
}

std::time_t own_log::getTimeStamp()
{
	return std::time(0);
}
