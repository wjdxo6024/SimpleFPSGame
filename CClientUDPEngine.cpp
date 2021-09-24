#include "CClientUDPEngine.h"

void CClientUDPEngine::err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CClientUDPEngine::err_display(const char *msg, LPVOID MsgDes)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	MsgDes = lpMsgBuf;
}

int CClientUDPEngine::recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}


bool CClientUDPEngine::Initialize()
{
	int retval;

	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
	{
		return false;
	}

	m_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
		return false;
	}

	// 멀티캐스트 TTL 설정
	int ttl = 128;
	retval = setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char*)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR)
	{
		err_quit("setsockopt()");
		return false;
	}
	// connect
	ZeroMemory(&m_remoteaddr, sizeof(m_remoteaddr));
	m_remoteaddr.sin_family = AF_INET;
	m_remoteaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	m_remoteaddr.sin_port = htons(REMOTEPORT);

	IsSendBufferFull = false;
	IsRecvBufferFull = false;

	IsRunning = true;
	return true;
}

bool CClientUDPEngine::Initialize(const char* serverip, int serverport)
{
	int retval;

	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
	{
		return false;
	}

	m_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
		return false;
	}

	// 멀티캐스트 TTL 설정
	int ttl = 128;
	retval = setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char*)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR)
	{
		err_quit("setsockopt()");
		return false;
	}
	// connect
	ZeroMemory(&m_remoteaddr, sizeof(m_remoteaddr));
	m_remoteaddr.sin_family = AF_INET;
	m_remoteaddr.sin_addr.s_addr = inet_addr(serverip);
	m_remoteaddr.sin_port = htons(serverport);

	IsSendBufferFull = false;
	IsRecvBufferFull = false;

	IsRunning = true;
	return true;
}


bool WINAPI CClientUDPEngine::Run()
{
	while (IsRunning) {
		// Send
		Send();

		Recieve();
	}
	return true;
}

bool CClientUDPEngine::Update()
{
	return true;
}

bool CClientUDPEngine::Send()
{
	if (!IsSendBufferFull)
		return false;
	int addrlen;
	int retval;

	len = strlen(m_Sendbuffer);
	if (m_Sendbuffer[len - 1] == '\n')
		m_Sendbuffer[len - 1] = '\0';
	if (strlen(m_Sendbuffer) == 0)
		return true;

	retval = sendto(m_sock, m_Sendbuffer, len, 0,
		(SOCKADDR *)&m_remoteaddr, sizeof(m_remoteaddr));
	if (retval == SOCKET_ERROR) {
		err_quit("send()");
		return false;
	}

	IsSendBufferFull = false;
	return true;
}

bool CClientUDPEngine::Recieve()
{
	if (IsRecvBufferFull)
		return false;

	int retval = 0;
	int addrlen = sizeof(m_remoteaddr);
	retval = recvfrom(m_sock, m_RecvBuffer, BUFSIZE, 0,
		(SOCKADDR *)&m_remoteaddr, &addrlen);
	if (retval == SOCKET_ERROR)
	{
		err_quit("recv()");
		return false;
	}

	else if (retval == 0)
		return false;

	m_RecvBuffer[retval] = '\0';

	IsRecvBufferFull = true;
	return true;
}

bool CClientUDPEngine::InputSendMessage(const char* data)
{
	if (IsSendBufferFull)
	{
		Send();
		IsSendBufferFull = false;
	}

	int i = 0;
	while (data[i] != '\0')
	{
		m_Sendbuffer[i] = data[i];
		i++;
	}
	m_Sendbuffer[i] = '\0';
	IsSendBufferFull = true;
	return true;
}

bool CClientUDPEngine::InputSendMessage(std::string& data)
{
	if (IsSendBufferFull)
	{
		Send();
		IsSendBufferFull = false;
	}

	int i = 0;
	while (data[i] != '\0')
	{
		m_Sendbuffer[i] = data[i];
		i++;
	}
	m_Sendbuffer[i] = '\0';
	IsSendBufferFull = true;
	return true;
}

bool CClientUDPEngine::GetRecvMessage(char* dest)
{
	//if (!IsRecvBufferFull)
	//	return false;

	int i = 0;
	while (m_RecvBuffer[i] != '\0')
	{
		dest[i] = m_RecvBuffer[i];
		i++;
	}
	dest[i] = '\0';
	memset(m_RecvBuffer, 0, BUFSIZE + 1);
	IsRecvBufferFull = false;
	return true;
}

bool CClientUDPEngine::GetRecvMessage(std::string& dest)
{
	//if (!IsRecvBufferFull)
	//	return false;

	dest = m_RecvBuffer;

	memset(m_RecvBuffer, 0, BUFSIZE + 1);
	IsRecvBufferFull = false;
	return true;
}

bool CClientUDPEngine::GetRecvMessageWithCode(char* dest, ProtocolDefine ProtocolCode)
{
	//if (!IsRecvBufferFull)
	//	return false;

	std::string TempBuffer = m_RecvBuffer;
	int tokenCode;
	std::stringstream ss;
	ss.str(TempBuffer);
	ss >> tokenCode;
	if (tokenCode == ProtocolCode)
	{
		int i = 0;
		while (m_RecvBuffer[i] != '\0')
		{
			dest[i] = m_RecvBuffer[i];
			i++;
		}
		dest[i] = '\0';
		memset(m_RecvBuffer, 0, BUFSIZE + 1);
		IsRecvBufferFull = false;
		return true;
	}
	else
		return false;
}

bool CClientUDPEngine::GetRecvMessageWithCode(std::string& dest, ProtocolDefine ProtocolCode)
{
	//if (!IsRecvBufferFull)
	//	return false;

	std::string TempBuffer = m_RecvBuffer;
	int tokenCode;
	std::stringstream ss;
	ss.str(TempBuffer);
	ss >> tokenCode;
	if (tokenCode == ProtocolCode)
	{
		dest = m_RecvBuffer;

		memset(m_RecvBuffer, 0, BUFSIZE + 1);
		IsRecvBufferFull = false;
	}
	else
		return false;
}

bool CClientUDPEngine::ShutDown()
{
	closesocket(m_sock);

	WSACleanup();
	memset(m_RecvBuffer, '\0', BUFSIZE + 1);
	memset(m_Sendbuffer, '\0', BUFSIZE + 1);
	IsRunning = false;
	IsSendBufferFull = false;
	IsRecvBufferFull = false;
	return true;
}

char* CClientUDPEngine::GetMsgBuffer()
{
	return m_RecvBuffer;
}

bool CClientUDPEngine::GetChattingString(std::wstring& msg)
{
	std::wstring tmp(m_RecvBuffer, &m_RecvBuffer[BUFSIZE]);
	msg = tmp;
	return true;
}

WSADATA& CClientUDPEngine::GetWSA()
{
	return m_wsa;
}

SOCKET& CClientUDPEngine::GetSocket()
{
	return m_sock;
}