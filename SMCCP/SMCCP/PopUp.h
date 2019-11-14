#pragma once
#include <string>
#include <Windows.h>



static class PopUp
{
public:
	enum class Result
	{
		Abort = 3,
		Cancel = 2,
		Continue = 11,
		Ignore = 5,
		No = 7,
		Ok = 1,
		Retry = 4,
		Again = 10,
		Yes = 6
	};

private:
	static int ResultToInt(Result r);
	static Result IntToResult(int i);

public:
	static int ShowMessageBox(std::string capt, std::string message, int type);
};

