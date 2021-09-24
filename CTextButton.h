#pragma once
#include "CText.h"
#include "ButtonEvent.h"
#include "CBox.h"
#include <windows.h>
class CTextButton
{
private:
	RECT m_CheckRect;
	CText m_ButtonText;
	ButtonEvent m_Event;
	bool IsShowRect;
	bool MouseInRect;

	// Debug �� �ڽ�
	CBox m_Box;
public:
	void OnInit(ButtonEventDefine Event, RECT rt, LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	void OnInit(ButtonEventDefine Event, int x, int y, int width, int height, LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	void OnRender(const char* Text, D3DXCOLOR TextColor);
	ButtonEventDefine OnUpdate(int x, int y);
	ButtonEventDefine OnUpdate(int x, int y, UINT message);
	void OnRelease();

	void SetTextSize(int Size);

private:
	bool SetEvent(ButtonEventDefine Event); // �̺�Ʈ ����
	bool CheckCursorInRect(int x, int y); // Ŀ�� üũ
	void SetRect(float top, float bottom, float left, float right); // Rect ����
	void SetRect(int top, int bottom, int left, int right); // Rect ����
};

