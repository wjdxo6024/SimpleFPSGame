#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <atlstr.h>
#include <cstring>
#include <chrono>
#include "CustomUtility.h"
#include "CClientEngine.h"
#include "CClientUDPEngine.h"
#include "MyDef.h"
#include "GameProtocol.h"

extern HWND g_hWnd;

class CClient
{
public:
	CClientEngine m_NetworkEngine;
private:
	std::wstring m_ClientNickName;

	int m_ClientState = 0;
	CClientUDPEngine m_MultiCastEngine;

	ClientState m_LoginState = BEFORELOGIN;
	ClientState m_ClientPlayState = BEFORELOGIN;

	std::thread clientTCPThread;
	std::thread clientUDPThread;
	HWND m_hWnd;

private:
	
public:
	// TCP
	bool Login(std::wstring Name, std::wstring& msg);
	bool Logout();
	bool RecvChatting(std::wstring& dest);
	bool SendChatting(std::wstring Msg);

	bool SendWithCode(std::wstring& Msg, int Code);
	bool SendCode(int Code);

	bool RequestKillDeathEvent(std::wstring& killer, int& killerTeam, std::wstring& deadMan, int& DeadMan);

	bool SendToServer(); // Engine���� Send �ۿ�
	bool ReceiveFromServer(); // Engine���� Receive �ۿ�
	// UDP
	bool LoginMultiCast();
	bool LogoutMultiCast();
	bool SendUDPToServer();
	bool ReceiveUDPFromServer();

	bool Run(); // �����͸� ���������� �޴´�.

	// Client ����
	bool Initialize(HWND hWnd); //
	bool ShutdownNetwork();
	bool SetState(int State);
	int GetState();

	void SetNickName(std::wstring NickName);
	std::wstring GetNickName();

	bool GetData(std::wstring& dest); // ���޵� ���ۿ� ���� ���ڿ��� �����´�.
};

