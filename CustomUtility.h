#pragma once
#include <string>
#include <cstring>
#include <atlstr.h>


int CustomStrlen(std::wstring String);
int CustomStrlen(std::string String);
int CustomStrlen(std::wstring String, int FontSize);

int GetCharacterSize(wchar_t Character, int FontSize);

std::wstring StringToWstring(const std::string& _src);

std::string WstringToString(const std::wstring& _src);
