#include "CustomUtility.h"

int CustomStrlen(std::wstring String)
{
	int Count = 0;
	for (std::wstring::iterator it = String.begin(); it != String.end(); it++)
	{
		// 한글 일경우
		if ((0xAC00 <= *it && *it <= 0xD7A3) || (*it > 44032 && *it < 55199))
			Count += 2;
		else
			Count++;
	}

	return Count;
}

// 한글 체크 부분이 제대로 작동 안됨 고칠 것
int CustomStrlen(std::string String)
{
	int Count = 0;
	for (std::string::iterator it = String.begin(); it != String.end(); it++)
	{
		// 한글 일경우
		if ((*it & 0x80) == 1)
			Count += 2;
		else
			Count++;
	}

	return Count;
}

int CustomStrlen(std::wstring String, int FontSize)
{
	int Count = 0;
	for (std::wstring::iterator it = String.begin(); it != String.end(); it++)
	{
		// 한글 일경우
		if ((12593 <= *it && *it <= 12686) || (*it > 44032 && *it < 55199))
			Count += FontSize;
		else
		{
			if (isupper(*it))
			{
				Count += FontSize;
			}
			else
				Count += FontSize / 2;
		}
	}

	return Count;
}

int GetCharacterSize(wchar_t Character, int FontSize)
{
	if ((12593 <= Character && Character <= 12686) || (Character > 44032 && Character < 55199))
		return FontSize;
	else
	{
		if (isupper(Character))
		{
			return FontSize;
		}
		else
			return FontSize / 2;
	}
}

std::wstring StringToWstring(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

std::string WstringToString(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};