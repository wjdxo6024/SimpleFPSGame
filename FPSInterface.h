#pragma once
#include <d3d9.h>
#include <vector>
#include "AmmoSystem.h"
#include "CrossHair.h"
#include "GameTimeRemaining.h"
#include "HPSystem.h"
#include "Weapon.h"
#include "Camera.h"

class FPSInterface
{
private:
	int m_WeaponState;
	HPSystem m_HP;
	Weapon* m_Weapon;
	GameTimeRemaining m_GameTimer;
	CrossHair m_CrossHair;

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, Weapon* Weapon);
	bool OnRender();
	int OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnRelease();

	void SelectWeapon(int SelectWeapon);
	void ShootWeapon();
	void ReloadWeapon();
};

