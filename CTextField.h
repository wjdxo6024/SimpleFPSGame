#pragma once
#include <iostream>
#include <d3d9.h>
#include "CText.h"
#include "CBox.h"
#include <Windows.h>
#include <Dimm.h>

#define INPUT_STRING_SIZE_MAX 256
#define INFINITY_STRING -1
class CTextField
{
private:
	CText m_TextInform;
#ifdef _DEBUG // Test
	std::string m_MousePos;
#endif
	CBox m_Box;
	std::wstring m_StringBuf;
	std::string m_OutputString;
	std::wstring m_wOutputString;
	RECT m_Rect;
	D3DXCOLOR m_TextColor;
	bool m_IsFocus;
	bool m_Is_AlwaysShowBox;
	HIMC m_hIMC;
	HWND m_hWnd;
	char m_InputWord[10];
	char m_resultWord[INPUT_STRING_SIZE_MAX];
	int m_LimitString = INFINITY_STRING;
private:
	void ClearText();
	void ShowText();
	void ShowCursor();

public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height);
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height, int LimitString);
	bool OnRender();
	bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos);
	bool OnUpdateClient(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos);
	bool OnRelease();

	bool OnFocusIn(POINT MousePos);
	bool OnFocusOut();
	bool IsFocus();

	bool HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos);

	bool GetOutputString(std::wstring& dest);

	bool LimitInputChar(int limit);

	bool IsBlank();
	bool AlwaysShowBox();
};

