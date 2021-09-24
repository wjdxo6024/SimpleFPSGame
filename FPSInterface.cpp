#include "FPSInterface.h"

bool FPSInterface::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, Weapon* Weapon)
{
	m_Weapon = Weapon;

	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;

	m_WeaponState = 0;
	m_HP.OnInit(m_pd3dDevice, m_hWnd);
	m_CrossHair.OnInit(m_pd3dDevice, m_hWnd);

	return true;
}

bool FPSInterface::OnRender()
{
	m_HP.OnRender();
	m_Weapon->OnRender();
	m_CrossHair.OnRender();
	return true;
}

int FPSInterface::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_HP.OnUpdate();
	m_CrossHair.OnUpdate(uMsg, wParam, lParam);

	switch (uMsg)
	{
	default:
		break;
	}
	return true;
}

bool FPSInterface::OnRelease()
{
	m_HP.OnRelease();
	m_CrossHair.OnRelease();

	return true;
}

void FPSInterface::SelectWeapon(int SelectWeapon)
{

}

void FPSInterface::ShootWeapon()
{

}

void FPSInterface::ReloadWeapon()
{

}