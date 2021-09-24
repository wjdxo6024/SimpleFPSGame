#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "GameProtocol.h"

#define MULTICASTIP "235.7.8.10"
#define REMOTEPORT  9000

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define BUFSIZE		512

class CClientUDPEngine
{
private:
	WSADATA m_wsa;
	SOCKET m_sock;
	SOCKADDR_IN m_peeraddr; // 데이터 통신에 사용할 변수
	SOCKADDR_IN m_remoteaddr;
	bool IsSendBufferFull;
	char m_Sendbuffer[BUFSIZE + 1];

	bool IsRecvBufferFull;
	char m_RecvBuffer[BUFSIZE + 1];

	bool IsRunning;
	int len;

private:
	void err_quit(const char *msg);
	void err_display(const char *msg, LPVOID MsgDes);
	int recvn(SOCKET s, char *buf, int len, int flags);
public:
	bool Initialize();
	bool Initialize(const char* serverip, int serverport);
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

