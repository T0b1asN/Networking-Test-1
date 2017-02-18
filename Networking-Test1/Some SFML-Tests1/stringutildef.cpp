#include "stringutil.h"

std::wstring str_to_wstr(std::string src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(src);
	return wide;
}

std::string wstr_to_str(std::wstring src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string narrow = converter.to_bytes(src);
	return narrow;
}