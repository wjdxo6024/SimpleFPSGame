#include "MultiPlayScene.h"

bool MultiPlayScene::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_hWnd = hWnd;

	RECT WindowRect;
	GetClientRect(m_hWnd, &WindowRect);

	m_CLoginDialog.OnInit(pd3dDevice, hWnd, WindowRect.right / 8, WindowRect.bottom / 8, WindowRect.right / 4 * 3, WindowRect.bottom / 4 * 3);
	Is_Login = false;

	// ���ͳ� ����
	m_Client = new CClient();
	m_Client->Initialize(hWnd);
	// �� ����
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

// ���� ���� �Ǹ� �α����� �ǽ��ϰ� �α��ο� �����ϸ� m_MultiPlayRoom ����
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
			// send �� recv �޴´� ���� ���� ���ʵ� send �� recv
			if (m_Client->Login(m_CLoginDialog.SummitNickName(), msg)) // �α��� ������
			{
				// send -> recv
				m_MultiPlayRoom->EnterRoom(); // ����Ʈ �ʱ�ȭ
				
				Is_Login = true;
			}
		}
		else if (Event == ButtonEventDefine::BackScene)
		{
			// Scene�� �ٲ�
		}
	}
	else // �α��� �Ǿ��� ��
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