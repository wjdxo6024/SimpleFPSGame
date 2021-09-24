#include "CInGameChat.h"

bool CInGameChat::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	RECT rt;
	GetClientRect(hWnd, &rt);

	m_ChattingField.OnInit(pd3dDevice, hWnd, rt.right / 24, rt.bottom / 16 * 14, rt.right / 2, 20);
	m_ChattingList.OnInit(pd3dDevice, hWnd, rt.right / 24, rt.bottom / 16 * 14 - (rt.bottom / 6), rt.right / 2, rt.bottom / 6);
	m_ChattingField.LimitInputChar(50);
	m_Focus = false;
	return true;
}

bool CInGameChat::OnInitInSingle(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	RECT rt;
	GetClientRect(hWnd, &rt);

	m_ChattingField.OnInit(pd3dDevice, hWnd, rt.right / 24, rt.bottom / 16 * 12, rt.right / 4, 20);
	m_ChattingList.OnInit(pd3dDevice, hWnd, rt.right / 24, rt.bottom / 16 * 10 - 20, rt.right / 4, rt.bottom / 4);
	m_ChattingField.LimitInputChar(50);
	m_Focus = false;
	return true;
}

bool CInGameChat::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool CInGameChat::OnUpdateInSingle(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool CInGameChat::OnRender()
{
	m_ChattingField.OnRender();
	m_ChattingList.OnRender();
	return true;
}

bool CInGameChat::OnRenderInSingle()
{
	m_ChattingField.OnRender();
	m_ChattingList.OnRender();
	return true;
}

bool CInGameChat::OnRelease()
{
	m_ChattingField.OnRelease();
	m_ChattingList.OnRelease();
	return true;
}

bool CInGameChat::OnReleaseInSingle()
{
	m_ChattingField.OnRelease();
	m_ChattingList.OnRelease();
	return true;
}

int CInGameChat::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_ChattingField.HandleKeyboard(uMsg, wParam, lParam);
	return true;
}

bool CInGameChat::IsFocus()
{
	return m_Focus;
}

bool CInGameChat::ChattingFocus()
{
	if (m_Focus)
	{
		m_Focus = false;
		m_ChattingField.AlwaysShowBox();
		m_ChattingList.AlwaysShowBox();
	}
	else
	{
		m_Focus = true;
		m_ChattingField.AlwaysShowBox();
		m_ChattingList.AlwaysShowBox();
	}

	return m_Focus;
}

bool CInGameChat::IsFieldBlank()
{
	return m_ChattingField.IsBlank();
}