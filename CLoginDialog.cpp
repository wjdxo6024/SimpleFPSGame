#include "CLoginDialog.h"

bool CLoginDialog::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;

	// 주의할 점 : 배치할 때, x, y는 다이얼로그의 줄여진 좌표로 계산됨. x, y = screen / 4
	m_dialogDomain.OnInit(pd3dDevice, hWnd, x, y, width, height, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
	
	m_Texts.OnInit(pd3dDevice, 2);

	m_NickNameField.OnInit(pd3dDevice, hWnd, x * 2.5, y * 3, width / 2, height / 16);
	m_NickNameField.AlwaysShowBox();
	m_NickNameField.LimitInputChar(10);

	m_SummitButton.OnInit(ButtonEventDefine::Summit, x * 2.25, y * 4, width / 4, height / 16, pd3dDevice, hWnd);
	m_ExitButton.OnInit(ButtonEventDefine::BackScene, x * 4.25, y * 4, width / 4, height / 16, pd3dDevice, hWnd);

#ifdef _DEBUG
	m_FreeParticipate.OnInit(ButtonEventDefine::ToMultiPlayRoom, x * 3.25, y * 6, width / 4, height / 16, pd3dDevice, hWnd);
#endif

	m_Focus = true;
	return true;
}

bool CLoginDialog::OnRender()
{
	m_dialogDomain.OnRender();

	// m_Texts.OnRender();

	m_NickNameField.OnRender();
	m_SummitButton.OnRender("Summit", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_ExitButton.OnRender("Exit", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#ifdef _DEBUG
	m_FreeParticipate.OnRender("Free Join For Debug", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

	return true;
}

ButtonEventDefine CLoginDialog::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos)
{
	ButtonEventDefine Event = ButtonEventDefine::Null;

	if (wParam != VK_RETURN) // 엔터 키 사용 제약
	{
		m_NickNameField.OnUpdateClient(uMsg, wParam, lParam, MousePos);
	}

	Event = m_SummitButton.OnUpdate(MousePos.x, MousePos.y, uMsg);
	if (Event != Null)
	{
		return Event;
	}
	Event = m_ExitButton.OnUpdate(MousePos.x, MousePos.y, uMsg);
	if (Event != Null)
	{
		return Event;
	}
#ifdef _DEBUG
	Event = m_FreeParticipate.OnUpdate(MousePos.x, MousePos.y, uMsg);
	if (Event != Null)
	{
		return Event;
	}
#endif
	if (Event != Null)
	{
		return Event;
	}
	return Null;
}

bool CLoginDialog::OnRelease()
{
	m_dialogDomain.OnRelease();
	m_NickNameField.OnRelease();
	m_SummitButton.OnRelease();
	m_ExitButton.OnRelease();
#ifdef _DEBUG
	m_FreeParticipate.OnRelease();
#endif
	return true;
}

std::wstring CLoginDialog::SummitNickName()
{
	std::wstring TempNickName;
	m_NickNameField.GetOutputString(TempNickName);
	return TempNickName;
}