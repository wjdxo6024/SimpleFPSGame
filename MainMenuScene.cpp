#include "MainMenuScene.h"

MainMenuScene::MainMenuScene()
{

}

MainMenuScene::~MainMenuScene()
{

}

MainMenuScene::MainMenuScene(const MainMenuScene& other)
{

}

bool MainMenuScene::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;

	RECT WindowRect;
	GetClientRect(m_hWnd, &WindowRect);
	
	// 주의 할점 top > bottom 이어야 한다.
	ToSingleButton.OnInit(ButtonEventDefine::ToSinglePlay, WindowRect.right / 8 * 3, WindowRect.bottom / 8 * 3, WindowRect.right / 4, WindowRect.bottom / 16,
		m_pd3dDevice, hWnd);
	ToMultiButton.OnInit(ButtonEventDefine::ToMultiPlay, WindowRect.right / 8 * 3, WindowRect.bottom / 8 * 4, WindowRect.right / 4, WindowRect.bottom / 16,
		m_pd3dDevice, hWnd);
	ToOptionButton.OnInit(ButtonEventDefine::ToOption, WindowRect.right / 8 * 3, WindowRect.bottom /  8 * 5, WindowRect.right / 4, WindowRect.bottom / 16,
		m_pd3dDevice, hWnd);
	ToExitButton.OnInit(ButtonEventDefine::ToExit, WindowRect.right / 8 * 3, WindowRect.bottom / 8 * 6, WindowRect.right / 4, WindowRect.bottom / 16,
		m_pd3dDevice, hWnd);

	m_Title.OnInit(pd3dDevice, 80);
	return true;
}

bool MainMenuScene::OnRender()
{
	ToSingleButton.OnRender("SinglePlay", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	ToMultiButton.OnRender("MultiPlay", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	ToOptionButton.OnRender("Option", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	ToExitButton.OnRender("Exit", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_Title.OnRender(150, 100, L"Marvelous Man Match", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	return true;
}

// Null일 경우 아무처리 하지 않음
ButtonEventDefine MainMenuScene::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_hWnd, &cursorPos);
	ButtonEventDefine Event = ButtonEventDefine::Null;

	Event = ToSingleButton.OnUpdate(cursorPos.x, cursorPos.y, uMsg);
	if (Event != Null)
	{
		return Event;
	}

	Event = ToMultiButton.OnUpdate(cursorPos.x, cursorPos.y, uMsg);
	if (Event != Null)
	{
		return Event;
	}

	Event = ToOptionButton.OnUpdate(cursorPos.x, cursorPos.y, uMsg);
	if (Event != Null)
	{
		return Event;
	}

	Event = ToExitButton.OnUpdate(cursorPos.x, cursorPos.y, uMsg);
	if (Event != Null)
	{
		return Event;
	}

	return Event;
}

void MainMenuScene::OnRelease()
{
	ToSingleButton.OnRelease();
	ToMultiButton.OnRelease();
	ToOptionButton.OnRelease();
	ToExitButton.OnRelease();
	m_Title.OnRelease();
}