#pragma once
#include <Windows.h>
#include <Dimm.h>

class CKeyBoard
{
private:
	bool Is_Upper;
	bool Is_Hangul;
public:
	bool Initialize();
	UINT GetMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool SetInput(WCHAR Input);
	
};

