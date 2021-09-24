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
	bool OnInit(); // �� �޼ҵ�
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND m_hWnd);
	bool OnRender();
	bool OnUpdate();
	bool OnRelease();

	bool PlayerReady(std::wstring Name);
	bool PlayerListRenew(std::vector<std::wstring>& List);
	bool PlayerLogIn(std::wstring Name, bool IsReady); // ��ǻ� �÷��̾ �ִ� �Լ�
	bool PlayerLogout(std::wstring Name); // �÷��̾ ���� �Լ�
	bool PlayerPressReady(std::wstring Name);

	std::vector<std::pair<std::wstring, bool>> GetUserList();
};

