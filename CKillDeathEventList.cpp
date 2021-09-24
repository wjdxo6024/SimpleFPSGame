#include "CKillDeathEventList.h"

bool CKillDeathEventList::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	RECT tempRT;
	GetClientRect(hWnd, &tempRT);
	m_rt.bottom = tempRT.bottom / 32 * 6;
	m_rt.top = tempRT.bottom / 32;
	m_rt.right = tempRT.right / 64 * 65;
	m_rt.left = tempRT.right / 64 * 36;

	m_Box.OnInit(pd3dDevice, hWnd, m_rt, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);

	m_KiilerText.OnInit(pd3dDevice, 2);
	m_DeathManText.OnInit(pd3dDevice, 2);
	m_ToKillText.OnInit(pd3dDevice, 2);

	return true;
}

bool CKillDeathEventList::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int height, int width)
{
	m_rt.bottom = x + height;
	m_rt.top = x;
	m_rt.right = y + height;
	m_rt.left = y;

	m_Box.OnInit(pd3dDevice, hWnd, m_rt, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);

	m_KiilerText.OnInit(pd3dDevice, 2);
	m_DeathManText.OnInit(pd3dDevice, 2);
	m_ToKillText.OnInit(pd3dDevice, 2);

	return true;
}

bool CKillDeathEventList::OnRender()
{
#ifdef _DEBUG
	m_Box.OnRender();
#endif
	std::list<std::pair<std::pair<std::wstring, int>, std::pair<std::wstring, int>>>::iterator it;
	int y = m_rt.top + 10;
	// void CText::OnRender(int x, int y, int width, int height, const char* string, D3DXCOLOR fontColor)
	int width = (m_rt.right - m_rt.left) / 3 - 10;
	for (it = m_Eventlist.begin(); it != m_Eventlist.end(); it++)
	{
		// 0 = Green 1 = Purple
		if (it->first.second == 0)
		{
			m_KiilerText.OnRender(m_rt.left + 5, y, width, 20, it->first.first.c_str(), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else
			m_KiilerText.OnRender(m_rt.left + 5, y, width, 20, it->first.first.c_str(), D3DXCOLOR(1.0f, 0.2f, 1.0f, 1.0f));

		m_DeathManText.OnRender(m_rt.left + 5 + width, y, width, 20, L" Killed ", D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		if (it->second.second == 0)
		{
			m_ToKillText.OnRender(m_rt.left + 5 + width * 2, y, width, 20, it->first.first.c_str(), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else
			m_ToKillText.OnRender(m_rt.left + 5 + width * 2, y, width, 20, it->first.first.c_str(), D3DXCOLOR(1.0f, 0.2f, 1.0f, 1.0f));

		y += 30;
	}
	return true;
}

bool CKillDeathEventList::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, float dwTime)
{
	return true;
}

bool CKillDeathEventList::OnRelease()
{
	m_Box.OnRelease();
	
	m_KiilerText.OnRelease();
	m_DeathManText.OnRelease();
	m_ToKillText.OnRelease();
	return true;
}

bool CKillDeathEventList::InsertEvent(std::wstring Killer, std::wstring DeadMan, int KillerTeam, int DeadManTeam)
{
	std::pair<std::wstring, int> TempKiller;
	TempKiller.first = Killer;
	TempKiller.second = KillerTeam;

	std::pair<std::wstring, int> TempDeadMan;
	TempDeadMan.first = DeadMan;
	TempDeadMan.second = DeadManTeam;

	std::pair<std::pair<std::wstring, int>, std::pair<std::wstring, int>> TempData;
	TempData.first = TempKiller;
	TempData.second = TempDeadMan;

	if (m_Eventlist.size() >= 3)
	{
		m_Eventlist.pop_front();
	}
	m_Eventlist.push_back(TempData);

	return true;
}