#pragma once
#include "MyDef.h"
#include "CTextButton.h"
#include "CSprite.h"
#include "CText.h"
#include "Event.h"

class MainMenuScene
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	CText m_Title;
	HWND m_hWnd;
	CTextButton ToSingleButton;
	CTextButton ToMultiButton;
	CTextButton ToOptionButton;
	CTextButton ToExitButton;

public:
	MainMenuScene();
	~MainMenuScene();
	MainMenuScene(const MainMenuScene& other);

	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	ButtonEventDefine OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnRelease();
};

