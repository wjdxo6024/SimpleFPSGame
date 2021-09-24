#pragma once
#include <d3d9.h>
#include "CText.h"

class HPSystem
{
private:
	int m_PlayerHP;
	int m_OutputTextPosX, m_OutputTextPosY;

	CText m_HPText;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	bool OnUpdate();
	bool OnUpdate(bool IsDamage, int Damage);
	bool OnRelease();

	int GetHP();
	void SetHP(int hp);
};

