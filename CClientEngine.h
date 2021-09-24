#pragma once
#ifndef _CCLIENT_H_
#define _CCLIENT_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <thread>
#include "GameProtocol.h"
#include "CustomUtility.h"

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define BUFSIZE		512

class CClientEngine
{
public:
	static char m_Sendbuffer[BUFSIZE + 1];
	static char m_RecvBuffer[BUFSIZE + 1];
	static SOCKET m_sock;
private:
	HANDLE hThread;
	WSADATA m_wsa;
	bool IsSendBufferFull;

	bool IsRecvBufferFull;

	bool IsRunning;
	int len;

	// UDP


private:
	void err_quit(const char *msg);
	void err_display(const char *msg, LPVOID MsgDes);
	int recvn(SOCKET s, char *buf, int len, int flags);
public:
	bool Initialize(HWND hWnd);
	bool Initialize(const char* serverip, int serverport, HWND hWnd);
	bool WINAPI Run();
	bool Update();
	bool ShutDown();

	bool Send();
	bool Recieve();

	bool InputSendMessage(const char* data);
	bool InputSendMessage(std::string& data);

	//bool InputSendMessageWithCode(const char* data, ProtocolDefine ProtocolCode);
	//bool InputSendMessageWithCode(std::string& data, ProtocolDefine ProtocolCode);

	bool GetRecvMessage(char* dest);
	bool GetRecvMessage(std::string& dest);

	bool GetRecvMessageWithCode(char* dest, ProtocolDefine ProtocolCode);
	bool GetRecvMessageWithCode(std::string& data, ProtocolDefine ProtocolCode);

	bool GetChattingString(std::wstring& msg);

	char* GetMsgBuffer();

	//-----------------
	WSADATA& GetWSA();
	SOCKET& GetSocket();
};

#endif
