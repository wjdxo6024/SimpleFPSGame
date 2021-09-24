#include "CKeyBoard.h"

bool CKeyBoard::Initialize()
{
	Is_Upper = false;
	Is_Hangul = false;

	return true;
}

UINT CKeyBoard::GetMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool CKeyBoard::SetInput(WCHAR Input)
{
	return true;
}
