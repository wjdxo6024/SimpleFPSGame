#include "AmmoSystem.h"

bool AmmoSystem::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;

	RECT rt;
	GetClientRect(m_hWnd, &rt);

	SetAmmoLoaded(0);
	SetAmmoLeft(0);
	
	m_Magazine = 0;

	m_AmmoText.OnInit(m_pd3dDevice, (rt.bottom / 30) + (rt.right / 30));
	m_OutputTextPosX = (rt.right / 24) * 20;
	m_OutputTextPosY = (rt.bottom) - (rt.bottom / 24 * 2);

	return true;
}

bool AmmoSystem::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int NumberOfMagazine)
{
	this->OnInit(pd3dDevice, hWnd);

	m_Magazine = NumberOfMagazine;

	return true;
}

bool AmmoSystem::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int NumberOfMagazine, int NumberOfAmmoLoaded, int NumberOfAmmoLeft)
{
	this->OnInit(pd3dDevice, hWnd, NumberOfMagazine);
	m_AmmoLoaded = NumberOfAmmoLoaded;
	m_AmmoLeft = NumberOfAmmoLeft;
	return true;
}

bool AmmoSystem::OnRender()
{
	wchar_t OutputAmmoText[12];
	wsprintf(OutputAmmoText, L"%3d / %3d", m_AmmoLoaded, m_AmmoLeft);

	m_AmmoText.OnRender(m_OutputTextPosX, m_OutputTextPosY, OutputAmmoText, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	return true;
}

bool AmmoSystem::OnUpdate()
{
	return true;
}

bool AmmoSystem::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool AmmoSystem::OnRelease()
{
	m_AmmoText.OnRelease();
	return true;
}

int AmmoSystem::GetAmmoLoaded()
{
	return m_AmmoLoaded;
}

void AmmoSystem::SetAmmoLoaded(int NumberOfAmmo)
{
	m_AmmoLoaded = NumberOfAmmo;
}

int AmmoSystem::GetAmmoLeft()
{
	return m_AmmoLeft;
}

void AmmoSystem::SetAmmoLeft(int NumberOfAmmo)
{
	m_AmmoLeft = NumberOfAmmo;
}

bool AmmoSystem::ShootAmmo()
{
	if (m_AmmoLoaded > 0)
	{
		m_AmmoLoaded--;
		return true;
	}
	return false;
}

bool AmmoSystem::ReloadAmmo()
{
	if (m_AmmoLoaded == m_Magazine)
		return false;

	if (m_AmmoLeft == 0)
	{
		return false;
	}
	else
	{
		int Temp = m_AmmoLoaded;
		m_AmmoLoaded = m_Magazine;
		m_AmmoLeft -= (m_Magazine - Temp);
		if (m_AmmoLeft < 0)
			m_AmmoLeft = 0;
	}
	return true;
}