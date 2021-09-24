#include "GameScene.h"

bool GameScene::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_gameState = GameState::MAINMENU;
	m_MainMenuScene.OnInit(pd3dDevice, hWnd);
	m_SinglePlayScene.OnInit(pd3dDevice, hWnd);
	m_MultiPlayScene.OnInit(pd3dDevice, hWnd);
	m_OptionScene.OnInit(pd3dDevice, hWnd);
	m_ExitScene.OnInit(pd3dDevice, hWnd);
	return true;
}

bool GameScene::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (m_gameState)
	{
	case GameState::MAINMENU:
	{
		ChangeScene(m_MainMenuScene.OnUpdate(uMsg, wParam, lParam));
		break;
	}
	case GameState::SINGLE:
	{
		m_SinglePlayScene.OnUpdate(uMsg, wParam, lParam);
		break;
	}
	case GameState::MULTI:
	{
		m_MultiPlayScene.OnUpdate(uMsg, wParam, lParam);
		break;
	}
	case GameState::OPTIONS:
	{
		m_OptionScene.OnUpdate(uMsg);
		break;
	}
	case GameState::EXIT:
	{
		m_ExitScene.OnUpdate(uMsg);
		break;
	}
	}
	return true;
}

bool GameScene::OnRender()
{
	switch (m_gameState)
	{
		case GameState::MAINMENU:
		{
			m_MainMenuScene.OnRender();
			break;
		}
		case GameState::SINGLE:
		{
			m_SinglePlayScene.OnRender();
			break;
		}
		case GameState::MULTI:
		{
			m_MultiPlayScene.OnRender();
			break;
		}
		case GameState::OPTIONS:
		{
			m_OptionScene.OnRender();
			break;
		}
		case GameState::EXIT:
		{
			m_ExitScene.OnRender();
			break;
		}
	}
	return true;
}

void GameScene::OnRelease()
{
	m_MainMenuScene.OnRelease();
	m_SinglePlayScene.OnRelease();
	m_MultiPlayScene.OnRelease();
	m_OptionScene.OnRelease();
	m_ExitScene.OnRelease();
}

void GameScene::ChangeScene(ButtonEventDefine Event)
{
	if (Event == ButtonEventDefine::ToSinglePlay)
	{
		m_gameState = GameState::SINGLE;
	}
	else if (Event == ButtonEventDefine::ToMultiPlay)
	{
		m_gameState = GameState::MULTI;
	}
	else if (Event == ButtonEventDefine::ToOption)
	{
		m_gameState = GameState::OPTIONS;
	}
	else if (Event == ButtonEventDefine::ToExit)
	{
		PostQuitMessage(0);
		return;
	}
}