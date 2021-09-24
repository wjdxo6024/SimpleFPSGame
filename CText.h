#pragma once
#include <string>
#include "MyDef.h"
class CText
{
private:
	std::string m_String;
	std::wstring m_wString;
	LPD3DXFONT m_Font;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	int m_fontsize;
	DWORD m_TextAlign = DT_NOCLIP;
public:
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, int fontsize);
	// void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, std::string string);
	void OnRender(int x, int y, const char* string, D3DXCOLOR fontColor);
	void OnRender(RECT& rc, const char* string, D3DXCOLOR fontColor);
	void OnRender(int x, int y, int width, int height, const char* string, D3DXCOLOR fontColor);

	void OnRender(int x, int y, const wchar_t* string, D3DXCOLOR fontColor);
	void OnRender(RECT& rc, const wchar_t* string, D3DXCOLOR fontColor);
	void OnRender(int x, int y, int width, int height, const wchar_t* string, D3DXCOLOR fontColor);
	void OnUpdate();
	void OnRelease();

	void SetColor();
	void SetFont(const char* font);
	void SetFontSize(int fontsize);
	void SetTextAlign(DWORD TextAlign);
	int GetFontSize();
};

