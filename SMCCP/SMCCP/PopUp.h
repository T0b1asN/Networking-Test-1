#pragma once
#include <string>
#include <Windows.h>



static class PopUp
{
public:
	static int ShowMessageBox(std::string capt, std::string message, int type);
};

