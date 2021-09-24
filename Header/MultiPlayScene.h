#pragma once
#include "MyDef.h"
#include "CClient.h"
#include "MultiPlayRoomScene.h"
#include "CLoginDialog.h"
#include "thread"
class MultiPlayScene
{
private:
	CClient* m_Client;
	MultiPlayRoomScene* m_MultiPlayRoom;
	CLoginDialog m_CLoginDialog;
	HWND m_hWnd;
	bool Is_Login;
	bool RunningThread = false;
private:
	//int 
	bool CreateThread(CClient* Client);
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	int OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnRelease();

	CClient* GetClient();
};

