#pragma once
#include "Event.h"
#include "MyDef.h"
class ExitScene
{
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	bool OnUpdate(UINT message);
	bool OnRelease();
};

