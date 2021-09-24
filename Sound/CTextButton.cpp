#include "CTextButton.h"

// 생각할 점 : 마우스의 이벤트에 따라 색상을 달리할 여러 방법을 고안
void CTextButton::OnInit(ButtonEventDefine Event, RECT rt, LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	IsShowRect = true;

	SetEvent(Event);
	m_CheckRect = rt;

	MouseInRect = false;
	m_ButtonText.OnInit(pd3dDevice, 20);
	m_Box.OnInit(pd3dDevice, hWnd, m_CheckRect, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
}

void CTextButton::OnInit(ButtonEventDefine Event, int x, int y, int width, int height, LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	IsShowRect = true;
	SetEvent(Event);
	SetRect(y, y + height, x, x + width);

	MouseInRect = false;
	m_ButtonText.OnInit(pd3dDevice, 20);

	//TODO : 텍스트 정렬 지정(나중에 변경할 수 있도록 메서드를 추가할 것
	m_ButtonText.SetTextAlign(DT_CENTER | DT_VCENTER);

	m_Box.OnInit(pd3dDevice, hWnd, m_CheckRect, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
}

void CTextButton::OnRender(const char* Text, D3DXCOLOR TextColor)
{
	if (MouseInRect)
	{
		m_ButtonText.OnRender(m_CheckRect, Text, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		m_ButtonText.OnRender(m_CheckRect, Text, TextColor);
	}

	if (IsShowRect)
	{
		m_Box.OnRender();
	}
}

ButtonEventDefine CTextButton::OnUpdate(int x, int y)
{
	if (CheckCursorInRect(x, y))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Test", L"Test", MB_OK);
#endif
		return this->m_Event.GetEvent();
	}

	return ButtonEventDefine::Null;
}

ButtonEventDefine CTextButton::OnUpdate(int x, int y, UINT message)
{
	if (message == WM_LBUTTONDOWN)
	{
		if (CheckCursorInRect(x, y))
		{
#ifdef _DEBUG
			MessageBox(NULL, L"Test", L"Test", MB_OK);
#endif
			return this->m_Event.GetEvent();
		}
	}
	else
	{
		// 텍스트 색상 바꾸기
		if (CheckCursorInRect(x, y))
		{
			MouseInRect = true;
		}
		else
			MouseInRect = false;
	}

	return ButtonEventDefine::Null;
}

void CTextButton::OnRelease()
{

}

bool CTextButton::SetEvent(ButtonEventDefine Event)
{
	this->m_Event.InitButtonEvent(Event);
	return true;
}

bool CTextButton::CheckCursorInRect(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	return PtInRect(&m_CheckRect, pt);
	/*if (x >= CheckRect.left && x <= CheckRect.right && y >= CheckRect.bottom && y <= CheckRect.top)
	{
		return true;
	}
	return false;*/
}

void CTextButton::SetRect(float top, float bottom, float left, float right)
{
	this->m_CheckRect.top = top;
	this->m_CheckRect.bottom = bottom;
	this->m_CheckRect.left = left;
	this->m_CheckRect.right = right;
}

void CTextButton::SetRect(int top, int bottom, int left, int right)
{
	this->m_CheckRect.top = top;
	this->m_CheckRect.bottom = bottom;
	this->m_CheckRect.left = left;
	this->m_CheckRect.right = right;
}

void CTextButton::SetTextSize(int Size)
{
	m_ButtonText.SetFontSize(Size);
}