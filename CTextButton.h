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

	// Debug 용 박스
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
	bool SetEvent(ButtonEventDefine Event); // 이벤트 설정
	bool CheckCursorInRect(int x, int y); // 커서 체크
	void SetRect(float top, float bottom, float left, float right); // Rect 설정
	void SetRect(int top, int bottom, int left, int right); // Rect 설정
};

