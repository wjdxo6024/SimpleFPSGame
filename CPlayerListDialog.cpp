#include "CPlayerListDialog.h"

bool CPlayerListDialog::OnInit()
{
	return false;
}

bool CPlayerListDialog::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_PlayerName.OnInit(pd3dDevice, 2);
	m_ReadyState.OnInit(pd3dDevice, 2);

	RECT tempRc;
	GetClientRect(hWnd, &tempRc);

	int x = tempRc.right / 24;
	int y = tempRc.bottom / 18 * 2;
	int height = tempRc.bottom / 18;
	int width = (tempRc.right - (tempRc.right / 24 * 2)) / 2;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_Box[i * 2 + j].OnInit(pd3dDevice, hWnd, x, y, width, height, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
			x += width;
		}
		x = tempRc.right / 24;
		y += height;
	}

	m_PlayerName.SetTextAlign(DT_CENTER | DT_VCENTER);
	m_ReadyState.SetTextAlign(DT_CENTER | DT_VCENTER);

	return true;
}

bool CPlayerListDialog::OnRender()
{
	// int x, int y, int width, int height, const wchar_t* string, D3DXCOLOR fontColor
	for (int i = 0; i < 16; i++)
	{
		m_Box[i].OnRender();
		int x = m_Box[i].GetRT().left;
		int y = m_Box[i].GetRT().top;
		int width = m_Box[i].GetRT().right - m_Box[i].GetRT().left;
		int height = m_Box[i].GetRT().bottom - m_Box[i].GetRT().top;

		if (i < m_Users.size())
		{
			m_PlayerName.OnRender(x, y, width / 2, height, m_Users[i].first.c_str(),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			if (m_Users[i].second)
			{
				m_ReadyState.OnRender(x + width / 2, y, width / 2, height, L"READY", D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			}
		}
	}

	return true;
}

bool CPlayerListDialog::OnUpdate()
{
	return true;
}

bool CPlayerListDialog::OnRelease()
{
	for (int i = 0; i < 16; i++)
	{
		m_Box[i].OnRelease();
	}

	m_PlayerName.OnRelease();
	m_ReadyState.OnRelease();

	return true;
}

// 작성하지 않음.
bool CPlayerListDialog::PlayerListRenew(std::vector<std::wstring>& List)
{
	return true;
}

bool CPlayerListDialog::PlayerLogIn(std::wstring Name, bool IsReady)
{
	if (m_Users.size() >= 16)
		return false;
	else
	{
		m_Users.push_back(std::make_pair(Name, IsReady));
		return true;
	}
}

bool CPlayerListDialog::PlayerLogout(std::wstring Name)
{
	auto it = std::find_if(m_Users.begin(), m_Users.end(),
		[&Name](const std::pair<std::wstring, bool>& element) {return element.first == Name; });
	if (it != m_Users.end())
	{
		m_Users.erase(it);
		return true;
	}

	return false;
}

bool CPlayerListDialog::PlayerPressReady(std::wstring Name)
{
	auto it = std::find_if(m_Users.begin(), m_Users.end(), 
		[&Name](const std::pair<std::wstring, bool>& element) {return element.first == Name; });
	if (it != m_Users.end())
	{
		it->second = !it->second;
		return true;
	}

	return false;
}

bool CPlayerListDialog::PlayerReady(std::wstring Name)
{
	auto it = std::find_if(m_Users.begin(), m_Users.end(),
		[&Name](const std::pair<std::wstring, bool>& element) {return element.first == Name; });
	if (it != m_Users.end())
	{
		return it->second;
	}

	return false;
}

std::vector<std::pair<std::wstring, bool>> CPlayerListDialog::GetUserList()
{
	return m_Users;
}