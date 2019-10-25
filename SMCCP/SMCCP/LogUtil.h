#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <ctime>
#include <time.h>

#ifdef _DEBUG
#define LOG_FILE_NAME "TEST_LOG.txt"
#else
#define LOG_FILE_NAME "LOG.txt"
#endif
//INFO: Log file doesn't work with multiple exe's opened from the same origin at the same time
namespace own_log
{
	//Creates a log file
	//returns an error code
	//TODO: explain error codes
	//Codes:
	//	0 = Ok
	//	1 = Log file already exists
	int CreateLog();

	//Append a string to the log
	//It will be in a new line
	//returns true if sucessful
	//ignore the second parameter
	bool AppendToLog(std::string whatToAppend, bool was = false);
	//same as above, but without time
	bool AppendToLogWOTime(std::string whatToAppend, bool was = false);

	//TODO: Functions to read the log
	std::string getTime();

	//writes stuff to the command window, if in Debug mode
	void pushMsgToCommandIfDebug(std::string msg);
}
