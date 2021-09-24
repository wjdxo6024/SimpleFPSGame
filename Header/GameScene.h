#pragma once
#include "MyDef.h"

// Scene
#include "MainMenuScene.h"
#include "LoadScene.h"
#include "OptionScene.h"
#include "SinglePlayScene.h"
#include "MultiPlayScene.h"
#include "ExitScene.h"

class GameScene
{
private:
	GameState m_gameState;
	LoadScene m_LoadScene;

	MainMenuScene m_MainMenuScene;
	SinglePlayScene m_SinglePlayScene;
	MultiPlayScene m_MultiPlayScene;
	OptionScene m_OptionScene;
	ExitScene m_ExitScene;

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;

public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnRender();
	void OnRelease();

private:
	void ChangeScene(ButtonEventDefine Event);
};

