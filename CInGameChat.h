#pragma once
#include "CTextField.h"
#include "CTextList.h"

class CInGameChat
{
private:
	bool m_Focus;
	CTextField m_ChattingField;
	CTextList m_ChattingList;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnInitInSingle(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);

	bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnUpdateInSingle(UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool OnRender();
	bool OnRenderInSingle();

	bool OnRelease();
	bool OnReleaseInSingle();

	int HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool IsFocus();
	bool ChattingFocus();
	bool IsFieldBlank();
};

