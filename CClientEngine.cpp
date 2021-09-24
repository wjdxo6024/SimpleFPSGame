#include "CClientEngine.h"

// 데이터 통신에 사용할 변수
char CClientEngine::m_RecvBuffer[BUFSIZE + 1];
char CClientEngine::m_Sendbuffer[BUFSIZE + 1];
SOCKET CClientEngine::m_sock;
char buf[BUFSIZE + 1];

void CClientEngine::err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CClientEngine::err_display(const char *msg, LPVOID MsgDes)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	MsgDes = lpMsgBuf;
}

int CClientEngine::recvn(SOCKET s, char *buf, int len, int flags)
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

bool CClientEngine::Initialize(HWND hWnd)
{
	int retval;
	
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
	{
		return false;
	}

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
		return false;
	}

	//u_long on = 1;
	//retval = ioctlsocket(m_sock, FIONBIO, &on);
	//if (retval == SOCKET_ERROR)
	//{
	//	MessageBox(NULL, L"ioctlsocket", L"Non-Blocking", MB_OK);
	//	PostQuitMessage(0); // 강제 종료
	//}

	//u_long sock_on = 1;
	//retval = ioctlsocket(m_sock, FIONBIO, &sock_on);
	//if (retval == SOCKET_ERROR)
	//{
	//	err_quit("ioctlsocket()");
	//	return false;
	//}
	// 타임아웃 설정
	//int optval = 50;
	//retval = setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval));
	//if (retval == SOCKET_ERROR)
	//{
	//	err_quit("setsockopt()");
	//	return false;
	//}

	retval = WSAAsyncSelect(m_sock, hWnd, WM_FDREAD, FD_READ | FD_WRITE);
	// connect
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	while (1)
	{
		retval = connect(m_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) // 진행중
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			else if (WSAGetLastError() == WSAEALREADY ||
				WSAGetLastError() == WSAEISCONN)
				break;
			else
				err_quit("connect()");
			return false;
		}
	}
	IsSendBufferFull = false;
	IsRecvBufferFull = false;

	IsRunning = true;
	return true;
}

bool CClientEngine::Initialize(const char* serverip, int serverport, HWND hWnd)
{
	int retval;

	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
	{
		return false;
	}

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
		return false;
	}

	// connect
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(serverip);
	serveraddr.sin_port = htons(serverport);
	retval = connect(m_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
		return false;
	}

	IsSendBufferFull = false;
	IsRecvBufferFull = false;

	IsRunning = true;
	return true;
}


bool WINAPI CClientEngine::Run()
{
	while (IsRunning) {
		// Send
		Send();

		Recieve();
	}
	return true;
}

bool CClientEngine::Update()
{
	return true;
}

bool CClientEngine::Send()
{
	if (!IsSendBufferFull)
		return false;
	int retval;

	len = strlen(m_Sendbuffer);
	if (m_Sendbuffer[len - 1] == '\n')
		m_Sendbuffer[len - 1] = '\0';
	if (strlen(m_Sendbuffer) == 0)
		return true;

	retval = send(m_sock, m_Sendbuffer, len, 0);
	if (retval == SOCKET_ERROR) {
		return false;
	}
	
	IsSendBufferFull = false;
	return true;
}

bool CClientEngine::Recieve()
{
	int retval = 0;

	retval = recv(m_sock, m_RecvBuffer, BUFSIZE, 0);
	if (retval == SOCKET_ERROR)
	{
		return false;
	}
	else if (retval == 0)
		return false;

	m_RecvBuffer[retval] = '\0';

	return true;
}

bool CClientEngine::InputSendMessage(const char* data)
{
	if (IsSendBufferFull)
	{
		Send();
		IsSendBufferFull = false;
	}

	int i = 0;
	while(data[i] != '\0')
	{
		m_Sendbuffer[i] = data[i];
		i++;
	}
	m_Sendbuffer[i] = '\0';
	IsSendBufferFull = true;
	return true;
}

bool CClientEngine::InputSendMessage(std::string& data)
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

bool CClientEngine::GetRecvMessage(char* dest)
{
	//if (!IsRecvBufferFull)
	//	return false;
	if (strlen(m_RecvBuffer) == 0) return false;

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

bool CClientEngine::GetRecvMessage(std::string& dest)
{
	//if (!IsRecvBufferFull)
	//	return false;
	dest = m_RecvBuffer;

	memset(m_RecvBuffer, 0, BUFSIZE + 1);
	IsRecvBufferFull = false;
	return true;
}

bool CClientEngine::GetRecvMessageWithCode(char* dest, ProtocolDefine ProtocolCode)
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

bool CClientEngine::GetRecvMessageWithCode(std::string& dest, ProtocolDefine ProtocolCode)
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

bool CClientEngine::ShutDown()
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

char* CClientEngine::GetMsgBuffer()
{
	return m_RecvBuffer;
}

bool CClientEngine::GetChattingString(std::wstring& msg)
{
	std::wstring tmp(m_RecvBuffer, &m_RecvBuffer[BUFSIZE]);
	msg = tmp;
	return true;
}

WSADATA& CClientEngine::GetWSA()
{
	return m_wsa;
}

SOCKET& CClientEngine::GetSocket()
{
	return m_sock;
}