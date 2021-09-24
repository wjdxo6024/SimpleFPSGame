#include "MultiPlayRoomScene.h"

bool MultiPlayRoomScene::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, CClient* parentClient)
{
	m_hWnd = hWnd;

	RECT rt;
	GetClientRect(hWnd, &rt);

	m_TextField.OnInit(pd3dDevice, hWnd, rt.right / 24, rt.bottom / 16 * 14, rt.right / 2, 20);
	m_TextList.OnInit(pd3dDevice, hWnd, rt.right / 24, rt.bottom / 16 * 14 - (rt.bottom / 6), rt.right / 2, rt.bottom / 6);
	m_TextField.AlwaysShowBox();
	m_TextList.AlwaysShowBox();
	m_TextField.LimitInputChar(50);

	m_ReadyButton.OnInit(ButtonEventDefine::Ready, rt.right / 24 * 15, rt.bottom / 16 * 14 - (rt.bottom / 6), rt.right / 24 * 9, rt.bottom / 6 + 20, pd3dDevice, hWnd);
	m_ReadyButton.SetTextSize(10);
	m_PlayerList.OnInit(pd3dDevice, hWnd);

	m_ParentClient = parentClient;
	return true;
}

bool MultiPlayRoomScene::OnRender()
{
	m_TextField.OnRender();
	m_TextList.OnRender();
	m_PlayerList.OnRender();

	if (IsReady)
	{
		m_ReadyButton.OnRender("NOT READY", D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	}
	else
		m_ReadyButton.OnRender("READY", D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	return true;
}

// ������� ���� ������ ó��
int MultiPlayRoomScene::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT& MousePos)
{
	m_TextField.OnUpdateClient(uMsg, wParam, lParam, MousePos);
	std::wstring tempString;
	if (wParam == VK_ESCAPE)
	{
		return MAINMENU;
	}

	// �÷��̾� ����Ʈ
	return 1;
}

// ����� �� �̺�Ʈ �߻��� �����ؾ� �ϴ� ������ �߽� ex)���� ��ư, �ؽ�Ʈ �ʵ�
int MultiPlayRoomScene::OnUpdateSend(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT& MousePos)
{
	std::wstring tempString;
	if (m_TextField.OnUpdate(uMsg, wParam, lParam, MousePos)) // �ʵ� �߻�
	{
		m_TextField.GetOutputString(tempString);
		//----------- ä�� -------------------
		m_ParentClient->SendChatting(tempString);
		m_ParentClient->SendToServer();
		return CHATTING;
		//------------------------------------

	}

	// �����ư
	if (m_ReadyButton.OnUpdate(MousePos.x, MousePos.y, uMsg) == ButtonEventDefine::Ready)
	{
		if (IsReady)
		{
			//m_PlayerList.PlayerPressReady(m_ParentClient->GetNickName());
			// �޽��� ���� NOTREADY
			m_ParentClient->SendCode(NOTREADY);
			m_ParentClient->SendToServer();
			//IsReady = false;
			return NOTREADY;
		}
		else
		{
			//m_PlayerList.PlayerPressReady(m_ParentClient->GetNickName());
			// �޽��� ���� READY

			m_ParentClient->SendCode(READY);
			m_ParentClient->SendToServer();
			//IsReady = true;
			return READY;
		}
	}

	// �÷��̾� ����Ʈ

	//// ���� �ƹ��� ���尡 ���� ��� �׳� 0�� ������.
	//m_ParentClient->SendCode(0);
	//m_ParentClient->SendToServer();
	return Null;
}

// ����� �� �޾ƾ� �ϴ� ������ ó�� ex) �÷��̾� ����, �ؽ�Ʈ����Ʈ
int MultiPlayRoomScene::OnUpdateRecv(UINT uMsg, WPARAM wParam, LPARAM lParam, std::wstring& dest, int& code)
{
			switch (code)
			{
			case LOGIN:
			{
				m_PlayerList.PlayerLogIn(dest, false);
				break;
			}
			case LOGOUT:
			{
				m_PlayerList.PlayerLogout(dest);
				break;
			}
			case READY: // READY�� ����� ������ ������� �����͸� �Ѹ���.
			{
				std::wstringstream ss(dest);
				std::wstring temp;
				ss >> temp; // code
				ss >> temp; // data
				m_PlayerList.PlayerPressReady(temp);
				IsReady = m_PlayerList.PlayerReady(m_ParentClient->GetNickName());
				break;
			}
			case NOTREADY:
			{
				std::wstringstream ss(dest);
				std::wstring temp;
				ss >> temp; // code
				ss >> temp; // data
				m_PlayerList.PlayerPressReady(temp);
				IsReady = m_PlayerList.PlayerReady(m_ParentClient->GetNickName());
				break;
			}
			case CHATTING:
			{
				std::wstringstream ss;
				std::wstringstream OutputString;
				ss.str(dest.c_str());
				std::wstring token;
				ss >> token; // Code
				ss >> token; // NickName
				OutputString << "[" << token << "]: ";
				// Data
				while (ss >> token)
				{
					OutputString << token;
					OutputString << ' ';
				}

				m_TextList.PushElement(OutputString.str());
				m_TextList.OnUpdate();
				break;
			}
			case INITCOMPLETE:
			{
				break;
			}
			case REQUESTUSERNAME:
			{
				break;
			}
			case STARTGAME:
			{
				break;
			}
			case RESULTGAME:
			{
				break;
			}
			case WAITGAME:
			{
				break;
			}
			default:
			{

				break;
			}
			}

	return Null;
}

bool MultiPlayRoomScene::OnRelease()
{
	m_TextField.OnRelease();
	m_TextList.OnRelease();
	m_PlayerList.OnRelease();
	m_ReadyButton.OnRelease();
	return true;
}

bool MultiPlayRoomScene::EnterRoom()
{
	// ��ȿ� �� �� ����Ʈ�� ������ �ʱ�ȭ �Ѵ�.
	m_ParentClient->SendCode(ProtocolDefine::REQUESTUSERNAME);
	m_ParentClient->SendToServer();

	bool IsRecv = false;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	while ((std::chrono::steady_clock::now() - start) < std::chrono::seconds(1)); // 5�ʰ� ��ٸ�
	m_ParentClient->ReceiveFromServer();

	std::wstring TempString;
	m_ParentClient->GetData(TempString);

	// ���� ����
	std::wstringstream ss(TempString);
	std::wstring NickName;
	bool IsReady;
	ss >> NickName; // Code ����
	while (ss >> NickName)
	{
		ss >> IsReady;
		m_PlayerList.PlayerLogIn(NickName, IsReady);
	}

	return true;
}