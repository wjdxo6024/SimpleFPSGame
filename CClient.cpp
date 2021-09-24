#include "CClient.h"

bool CClient::Login(std::wstring Name, std::wstring& msg)
{
	if (m_ClientNickName.size() != 0)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"�̹� �г����� ������ �α��ε� �����Դϴ�!", L"Error", MB_OK);
#endif
		msg = L"�̹� �г����� ������ �α��ε� �����Դϴ�!";
		return false;
	}
	// �г��� string���� ��ȯ
	std::ostringstream stringstream;
	std::string TempName;
	TempName = WstringToString(Name);
	stringstream << "11 " << TempName; // 11 LOGIN
	std::string tmpNickName = stringstream.str();
	// �α��� �ǽ�
	char RecvData[BUFSIZE + 1];
	std::string TempRecvData;
	if (!m_NetworkEngine.Initialize(m_hWnd))
	{
		MessageBox(NULL, L"��Ʈ��ũ ���ӿ� �����Ͽ����ϴ�.", L"Error", MB_OK);
		msg = L"��Ʈ��ũ ���ӿ� �����Ͽ����ϴ�.";
		m_NetworkEngine.ShutDown();
		return false;
	}
	if (m_NetworkEngine.InputSendMessage(tmpNickName.c_str())) // �г��� ���ۿ� �Է�
	{
		if (m_NetworkEngine.Send())
		{
			// ������ �ޱ�
			bool IsRecv = false;
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			while ((std::chrono::steady_clock::now() - start) < std::chrono::seconds(5)) // 5�ʰ� ��ٸ�
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
				MessageBox(NULL, L"�����͸� ���� ���߽��ϴ�.", L"Error", MB_OK);
				msg = L"�����͸� ���� ���߽��ϴ�.";
				m_NetworkEngine.ShutDown();
				return false;
			}

			std::stringstream ss(TempRecvData);
			int token;
			ss >> token;

			// �α��� 
			if (token == 11) // �α��� ����
			{
				goto SUCCESS;
			}
			else if (token == 10) // ���� �÷��̾ ��� �����ִ�.
			{
				MessageBox(NULL, L"���� �Է��� �г����� ���� �÷��̾ �����մϴ�.", L"Error", MB_OK);
				msg = L"���� �Է��� �г����� ���� �÷��̾ �����մϴ�.";
				m_NetworkEngine.ShutDown();
				return false;
			}
			else
			{
				MessageBox(NULL, L"������ �� �ڸ��� �����ϴ�.", L"Error", MB_OK);
				msg = L"������ �� �ڸ��� �����ϴ�.";
				m_NetworkEngine.ShutDown();
				return false;
			}
		}
		else
		{
			MessageBox(NULL, L"�α��ο� �����Ͽ����ϴ�.", L"Error", MB_OK);
			msg = L"�α��ο� �����Ͽ����ϴ�.";
			m_NetworkEngine.ShutDown();
			return false;
		}

	}
	else
	{
		MessageBox(NULL, L"������ ���ۿ� �����Ͽ����ϴ�.", L"Error", MB_OK);
		msg = L"������ ���ۿ� �����Ͽ����ϴ�.";
		m_NetworkEngine.ShutDown();
		return false;
	}

	return true;
	// �α��� ����

SUCCESS:
	m_ClientNickName = Name;
	m_LoginState = INROOM;
	m_ClientPlayState = INROOM;
	// UDP ���ᵵ ���⿡


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

// ä��
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
// Engine���� Send �ۿ�
bool CClient::ReceiveFromServer()
{
	return m_NetworkEngine.Recieve();
}// Engine���� Receive �ۿ�

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