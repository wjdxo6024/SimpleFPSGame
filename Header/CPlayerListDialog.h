#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "CDialog.h"
#include "CText.h"
#include "CBox.h"

class CPlayerListDialog : public CDialog
{
private:
	std::vector<std::pair<std::wstring, bool>> m_Users;
	CText m_PlayerName;
	CText m_ReadyState;
	CBox m_Box[16];
public:
	bool OnInit(); // 빈 메소드
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND m_hWnd);
	bool OnRender();
	bool OnUpdate();
	bool OnRelease();

	bool PlayerReady(std::wstring Name);
	bool PlayerListRenew(std::vector<std::wstring>& List);
	bool PlayerLogIn(std::wstring Name, bool IsReady); // 사실상 플레이어를 넣는 함수
	bool PlayerLogout(std::wstring Name); // 플레이어를 빼는 함수
	bool PlayerPressReady(std::wstring Name);

	std::vector<std::pair<std::wstring, bool>> GetUserList();
};

