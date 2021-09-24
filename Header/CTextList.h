#pragma once
#include <iostream>
#include <d3d9.h>
#include <Windows.h>
#include <vector>
#include "CText.h"
#include "CBox.h"
#include "CustomUtility.h"

class CTextList
{
private:
	std::vector<std::wstring> m_StringList;
	CBox m_Box;
	RECT m_Rect;
	HWND m_hWnd;
	CText m_TextInform;
	D3DXCOLOR m_TextColor;
	int m_ListCurShow; // 현재 출력할 텍스트 리스트 
	int ListSize;

	bool m_IsFocus;
	bool m_Is_AlwaysShowBox;
	// 리스트 슬라이더 나중에 추가
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height);
	bool OnRender();
	bool OnUpdate();
	bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos);
	bool OnRelease();

	std::vector<std::wstring>& GetList();
	std::wstring& GetListByIndex(int index);
	void PushElement(std::wstring dest);
	bool DeleteElement(int index);

	void ClearList();
	void SetTextColor(D3DXCOLOR TextColor);

	bool SetFocus();
	bool AlwaysShowBox();
};

