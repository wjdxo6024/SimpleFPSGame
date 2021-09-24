#pragma once
#include <d3d9.h>
#include "CText.h"

class AmmoSystem
{
private:
	int m_OutputTextPosX, m_OutputTextPosY;
	int m_AmmoLoaded;
	int m_AmmoLeft;
	int m_Magazine;
	CText m_AmmoText;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;

public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int NumberOfMagazine);
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int NumberOfMagazine, int NumberOfAmmoLoaded, int NumberOfAmmoLeft);
	bool OnRender();

	bool OnUpdate();
	bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	bool OnRelease();

	bool ShootAmmo();
	bool ReloadAmmo();

	int GetAmmoLoaded();
	void SetAmmoLoaded(int NumberOfAmmo);

	int GetAmmoLeft();
	void SetAmmoLeft(int NumberOfAmmo);
};

