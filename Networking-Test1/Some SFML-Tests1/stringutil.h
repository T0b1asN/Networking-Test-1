#pragma once
#include <locale>
#include <codecvt>
#include <string>

std::wstring str_to_wstr(std::string src);
std::string wstr_to_str(std::wstring src);
