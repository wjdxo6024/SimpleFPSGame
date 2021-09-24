#include "MultiPlayScene.h"

bool MultiPlayScene::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_hWnd = hWnd;

	RECT WindowRect;
	GetClientRect(m_hWnd, &WindowRect);

	m_CLoginDialog.OnInit(pd3dDevice, hWnd, WindowRect.right / 8, WindowRect.bottom / 8, WindowRect.right / 4 * 3, WindowRect.bottom / 4 * 3);
	Is_Login = false;

	// 인터넷 연결
	m_Client = new CClient();
	m_Client->Initialize(hWnd);
	// 룸 생성
	m_MultiPlayRoom = new MultiPlayRoomScene;
	m_MultiPlayRoom->OnInit(pd3dDevice, hWnd, m_Client);
	return true;
}

bool MultiPlayScene::OnRender()
{
	if (!Is_Login)
	{
		m_CLoginDialog.OnRender();
	}
	else
	{
		m_MultiPlayRoom->OnRender();
	}
	return true;
}

// 씬이 시작 되면 로그인을 실시하고 로그인에 성공하면 m_MultiPlayRoom 실행
int MultiPlayScene::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT MousePos;
	ButtonEventDefine Event;
	GetCursorPos(&MousePos);
	ScreenToClient(m_hWnd, &MousePos);
	if (!Is_Login)
	{
		Event = m_CLoginDialog.OnUpdate(uMsg, wParam, lParam, MousePos);
		if (Event == ToMultiPlayRoom)
		{
			Is_Login = true;
		}
		if (Event == Summit)
		{
			std::wstring msg;
			// send 후 recv 받는다 따라서 다음 차례도 send 후 recv
			if (m_Client->Login(m_CLoginDialog.SummitNickName(), msg)) // 로그인 성공시
			{
				// send -> recv
				m_MultiPlayRoom->EnterRoom(); // 리스트 초기화
				
				Is_Login = true;
			}
		}
		else if (Event == ButtonEventDefine::BackScene)
		{
			// Scene을 바꿈
		}
	}
	else // 로그인 되었을 때
	{
		std::wstringstream ss;
		std::wstringstream output;
		std::wstring RecvMsg;
		int Code;
		int retval;
		m_MultiPlayRoom->OnUpdate(uMsg, wParam, lParam, MousePos);
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
			m_Client->ReceiveFromServer();
			if (m_Client->GetData(RecvMsg))
			{
				ss >> Code;
				m_MultiPlayRoom->OnUpdateRecv(uMsg, wParam, lParam, RecvMsg, Code);
			}
			break;
		}
		}
		m_MultiPlayRoom->OnUpdateSend(uMsg, wParam, lParam, MousePos);
	}
	return true;
}

bool MultiPlayScene::OnRelease()
{
	if (m_MultiPlayRoom != NULL)
	{
		m_MultiPlayRoom->OnRelease();
		delete m_MultiPlayRoom;
	}

	if (m_Client != NULL)
	{
		if (Is_Login)
			m_Client->Logout();
		else
			m_Client->ShutdownNetwork();

		delete m_Client;
	}

	return true;
}

CClient* MultiPlayScene::GetClient()
{
	return m_Client;
}

bool MultiPlayScene::CreateThread(CClient* Client)
{
	std::thread recvThread([&Client](){
		while (1)
		{
			if (!Client->ReceiveFromServer())
				return false;
		}
	});

	recvThread.join();
	return true;
}