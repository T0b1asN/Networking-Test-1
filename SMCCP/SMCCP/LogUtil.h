#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <ctime>
#include <time.h>

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
	int CreateLog();

	//Append string to log (new line)
	//second parameter: Should the time be added?
	//returns true if successful
	bool AppendToLog(std::string whatToAppend, bool addTime = true);

	//TODO: Functions to read the log
	std::string getTime();
	std::time_t getTimeStamp();

	//writes stuff to the command window, if in Debug mode
	void pushMsgToCommandIfDebug(std::string msg);
}
