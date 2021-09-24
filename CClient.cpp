#include "CClient.h"

bool CClient::Login(std::wstring Name, std::wstring& msg)
{
	if (m_ClientNickName.size() != 0)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"이미 닉네임을 가지고 로그인된 상태입니다!", L"Error", MB_OK);
#endif
		msg = L"이미 닉네임을 가지고 로그인된 상태입니다!";
		return false;
	}
	// 닉네임 string으로 변환
	std::ostringstream stringstream;
	std::string TempName;
	TempName = WstringToString(Name);
	stringstream << "11 " << TempName; // 11 LOGIN
	std::string tmpNickName = stringstream.str();
	// 로그인 실시
	char RecvData[BUFSIZE + 1];
	std::string TempRecvData;
	if (!m_NetworkEngine.Initialize(m_hWnd))
	{
		MessageBox(NULL, L"네트워크 접속에 실패하였습니다.", L"Error", MB_OK);
		msg = L"네트워크 접속에 실패하였습니다.";
		m_NetworkEngine.ShutDown();
		return false;
	}
	if (m_NetworkEngine.InputSendMessage(tmpNickName.c_str())) // 닉네임 버퍼에 입력
	{
		if (m_NetworkEngine.Send())
		{
			// 데이터 받기
			bool IsRecv = false;
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			while ((std::chrono::steady_clock::now() - start) < std::chrono::seconds(5)) // 5초간 기다림
			{
				if (m_NetworkEngine.Recieve())
				{
					m_NetworkEngine.GetRecvMessage(TempRecvData);
					IsRecv = true;
					break;
				}
				else
					continue;
			}
			
			if (!IsRecv)
			{
				MessageBox(NULL, L"데이터를 받지 못했습니다.", L"Error", MB_OK);
				msg = L"데이터를 받지 못했습니다.";
				m_NetworkEngine.ShutDown();
				return false;
			}

			std::stringstream ss(TempRecvData);
			int token;
			ss >> token;

			// 로그인 
			if (token == 11) // 로그인 성공
			{
				goto SUCCESS;
			}
			else if (token == 10) // 현재 플레이어가 모두 꽉차있다.
			{
				MessageBox(NULL, L"현재 입력한 닉네임을 가진 플레이어가 존재합니다.", L"Error", MB_OK);
				msg = L"현재 입력한 닉네임을 가진 플레이어가 존재합니다.";
				m_NetworkEngine.ShutDown();
				return false;
			}
			else
			{
				MessageBox(NULL, L"서버에 빈 자리가 없습니다.", L"Error", MB_OK);
				msg = L"서버에 빈 자리가 없습니다.";
				m_NetworkEngine.ShutDown();
				return false;
			}
		}
		else
		{
			MessageBox(NULL, L"로그인에 실패하였습니다.", L"Error", MB_OK);
			msg = L"로그인에 실패하였습니다.";
			m_NetworkEngine.ShutDown();
			return false;
		}

	}
	else
	{
		MessageBox(NULL, L"데이터 전송에 실패하였습니다.", L"Error", MB_OK);
		msg = L"데이터 전송에 실패하였습니다.";
		m_NetworkEngine.ShutDown();
		return false;
	}

	return true;
	// 로그인 성공

SUCCESS:
	m_ClientNickName = Name;
	m_LoginState = INROOM;
	m_ClientPlayState = INROOM;
	// UDP 연결도 여기에


	return true;
}

bool CClient::Logout()
{
	std::string TempMsg("9 ");
	TempMsg += WstringToString(m_ClientNickName);
	m_NetworkEngine.InputSendMessage(TempMsg);
	m_NetworkEngine.Send();

	m_LoginState = ClientState::BEFORELOGIN;
	m_NetworkEngine.ShutDown();
	return true;
}

bool CClient::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	return true;
}

bool CClient::Run()
{
	if (m_LoginState == INLOGIN)
	{
		m_LoginState == INROOM;
	}
	return true;
}

bool CClient::SetState(int State)
{
	if (State >= 1000 && State <= ClientState::INPLAYING)
	{
		m_ClientState = State;
		return true;
	}
	return false;
}

bool CClient::ShutdownNetwork()
{
	m_NetworkEngine.ShutDown();
	return true;
}

int CClient::GetState()
{
	return m_ClientPlayState;
}

void CClient::SetNickName(std::wstring NickName)
{
	m_ClientNickName = NickName;
}

std::wstring CClient::GetNickName()
{
	return m_ClientNickName;
}

bool CClient::GetData(std::wstring& dest)
{
	std::string TempMsg;
	if (m_NetworkEngine.GetRecvMessage(TempMsg))
	{
		dest = StringToWstring(TempMsg);
		return true;
	}
	else
	{
		return false;
	}
}

// 채팅
bool CClient::RecvChatting(std::wstring& dest)
{
	std::string TempMsg;
	if (m_NetworkEngine.GetRecvMessageWithCode(TempMsg, CHATTING))
	{
		dest = StringToWstring(TempMsg);
		return true;
	}
	else
	{
		return false;
	}
}

bool CClient::SendChatting(std::wstring Msg)
{
	std::stringstream ss;
	std::string TempMsg;

	TempMsg = WstringToString(m_ClientNickName);
	ss << "14 " << TempMsg << " ";
	TempMsg = WstringToString(Msg);
	ss << TempMsg;

	return m_NetworkEngine.InputSendMessage(ss.str().c_str());
}

bool CClient::SendWithCode(std::wstring& Msg, int Code)
{
	std::stringstream ss;
	std::string TempMsg;

	TempMsg = WstringToString(m_ClientNickName);
	ss << Code << " " << TempMsg << " ";
	TempMsg = WstringToString(Msg);
	ss << TempMsg;

	return m_NetworkEngine.InputSendMessage(ss.str().c_str());
}

bool CClient::SendCode(int Code)
{
	std::stringstream ss;
	std::string TempMsg;

	TempMsg = WstringToString(m_ClientNickName);
	ss << Code << " " << TempMsg;

	if (Code == 0)
	{
		TempMsg = "";
		return m_NetworkEngine.InputSendMessage(TempMsg.c_str());
	}

	return m_NetworkEngine.InputSendMessage(ss.str().c_str());
}

bool CClient::SendToServer()
{
	return m_NetworkEngine.Send();
}
// Engine에서 Send 작용
bool CClient::ReceiveFromServer()
{
	return m_NetworkEngine.Recieve();
}// Engine에서 Receive 작용

// UDP
bool CClient::LoginMultiCast()
{
	m_MultiCastEngine.Initialize();

	return true;
}

bool CClient::LogoutMultiCast()
{
	m_MultiCastEngine.ShutDown();
	return true;
}

bool CClient::SendUDPToServer()
{
	return true;
}
bool CClient::ReceiveUDPFromServer()
{
	return true;
}