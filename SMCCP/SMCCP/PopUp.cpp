#include "PopUp.h"

int PopUp::ShowMessageBox(std::string capt, std::string message, int type)
{
	std::wstring msg_temp;
	msg_temp.assign(message.begin(), message.end());
	LPCTSTR msg = msg_temp.c_str();

	std::wstring cap_temp;
	cap_temp.assign(capt.begin(), capt.end());
	LPCTSTR cap = cap_temp.c_str();

	return MessageBox(0, msg, cap, type);
}
