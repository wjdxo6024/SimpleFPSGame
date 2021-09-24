#pragma once
#include <d3d9.h>
#include "AmmoSystem.h"
#include "WeaponObject.h"
#include "Sound3D.h"
#include "Sound.h"
class Weapon
{
private:
	int m_WeaponDamage;
	int m_WeaponMagazine;
	
	WeaponObject m_WeaponModel;

	Sound m_ShootSound;
	Sound m_ReloadSound;
	Sound m_OringSound;
	AmmoSystem m_WeaponAmmoSystem;

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;
public:
	virtual bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	virtual bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int Damage, int WeaponMagazine);
	virtual bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, 
		const char* ShootSoundPath,
		const char* ReloadSoundPath,
		const char* OringSoundPath,
		int Damage, int WeaponMagazine);
	virtual bool OnRender();
	virtual bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool OnRelease();
	
	virtual bool ShootWeaponAnimate(float Time);
	virtual bool LoadWeaponAnimate(float Time);
	virtual bool ReloadAnimate(float Time);

	virtual bool Load(float SoundX, float SoundY, float SoundZ);
	virtual bool Reload(float SoundX, float SoundY, float SoundZ);
	virtual bool Shoot(float SoundX, float SoundY, float SoundZ);

	virtual int HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetDamage(int Damage);
	void SetMagazine(int Magazine);

	int GetLeftAmmo();
	int GetLoadAmmo();
	void SetLeftAmmo(int LeftAmmo);
	void SetLoadAmmo(int LoadAmmo);

	void SetListener(float x, float y, float z);
};

