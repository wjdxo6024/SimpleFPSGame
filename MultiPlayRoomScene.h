#pragma once
#include "MyDef.h"
#include "CTextField.h"
#include "CTextList.h"
#include "CText.h"
#include "CClient.h"
#include "CTextButton.h"
#include "CPlayerListDialog.h"

class MultiPlayRoomScene
{
private:
	CClient* m_ParentClient; // MultiPlayScene�� Ŭ���̾�Ʈ, Delete���� �� ��.
	CTextField m_TextField;
	CTextList m_TextList;

	CTextButton m_ReadyButton;
	bool IsReady;

	CText m_TextOut;
	CPlayerListDialog m_PlayerList;
	HWND m_hWnd;
	std::vector<std::pair<std::string, bool>> m_PlayerNames;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, CClient* parentClient);
	bool OnRender();
	int OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT& MousePos);
	
	int OnUpdateSend(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT& MousePos);
	
	int OnUpdateRecv(UINT uMsg, WPARAM wParam, LPARAM lParam, std::wstring& dest, int& code);
	
	bool OnRelease();

	bool EnterRoom();
};

