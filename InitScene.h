#pragma once
#include "CText.h"
#include "CSprite.h"
#include "MyDef.h"

class InitScene
{
private:
	CText m_Text;
	CSprite m_Sprite;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	bool OnUpdate();
	bool OnRelease();
};

