#pragma once
#include "CText.h"
#include "CBox.h"
#include <list>
class CKillDeathEventList
{
private:
	RECT m_rt;
	/* 
	First = Killer.First -> Name
			Killer.Second -> Team
	Second = DeadMan.First -> Name
			 DeadMan.Second -> Team
	*/
	CBox m_Box;

	std::list<std::pair<std::pair<std::wstring, int>, std::pair<std::wstring, int>>> m_Eventlist;
	CText m_KiilerText;
	CText m_DeathManText;
	CText m_ToKillText; // 노란색 중간
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int height, int width);

	bool OnRender();
	bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, float dwTime);
	bool OnRelease();

	bool InsertEvent(std::wstring Killer, std::wstring DeadMan, int KillerTeam, int DeadManTeam);
};

