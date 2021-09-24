#pragma once
#include "MyDef.h"
class OptionScene
{
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	bool OnUpdate(UINT message);
	bool OnRelease();
};

