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

// 통신하지 않을 데이터 처리
int MultiPlayRoomScene::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT& MousePos)
{
	m_TextField.OnUpdateClient(uMsg, wParam, lParam, MousePos);
	std::wstring tempString;
	if (wParam == VK_ESCAPE)
	{
		return MAINMENU;
	}

	// 플레이어 리스트
	return 1;
}

// 통신할 때 이벤트 발생시 전송해야 하는 데이터 발신 ex)레디 버튼, 텍스트 필드
int MultiPlayRoomScene::OnUpdateSend(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT& MousePos)
{
	std::wstring tempString;
	if (m_TextField.OnUpdate(uMsg, wParam, lParam, MousePos)) // 필드 발생
	{
		m_TextField.GetOutputString(tempString);
		//----------- 채팅 -------------------
		m_ParentClient->SendChatting(tempString);
		m_ParentClient->SendToServer();
		return CHATTING;
		//------------------------------------

	}

	// 레디버튼
	if (m_ReadyButton.OnUpdate(MousePos.x, MousePos.y, uMsg) == ButtonEventDefine::Ready)
	{
		if (IsReady)
		{
			//m_PlayerList.PlayerPressReady(m_ParentClient->GetNickName());
			// 메시지 전달 NOTREADY
			m_ParentClient->SendCode(NOTREADY);
			m_ParentClient->SendToServer();
			//IsReady = false;
			return NOTREADY;
		}
		else
		{
			//m_PlayerList.PlayerPressReady(m_ParentClient->GetNickName());
			// 메시지 전달 READY

			m_ParentClient->SendCode(READY);
			m_ParentClient->SendToServer();
			//IsReady = true;
			return READY;
		}
	}

	// 플레이어 리스트

	//// 만약 아무런 샌드가 없을 경우 그냥 0을 보낸다.
	//m_ParentClient->SendCode(0);
	//m_ParentClient->SendToServer();
	return Null;
}

// 통신할 때 받아야 하는 데이터 처리 ex) 플레이어 보드, 텍스트리스트
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
			case READY: // READY는 방안의 유저를 대상으로 데이터를 뿌린다.
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
	// 방안에 들어갈 때 리스트의 내용을 초기화 한다.
	m_ParentClient->SendCode(ProtocolDefine::REQUESTUSERNAME);
	m_ParentClient->SendToServer();

	bool IsRecv = false;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	while ((std::chrono::steady_clock::now() - start) < std::chrono::seconds(1)); // 5초간 기다림
	m_ParentClient->ReceiveFromServer();

	std::wstring TempString;
	m_ParentClient->GetData(TempString);

	// 분해 시작
	std::wstringstream ss(TempString);
	std::wstring NickName;
	bool IsReady;
	ss >> NickName; // Code 제거
	while (ss >> NickName)
	{
		ss >> IsReady;
		m_PlayerList.PlayerLogIn(NickName, IsReady);
	}

	return true;
}