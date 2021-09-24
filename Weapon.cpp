#include "Weapon.h"

bool Weapon::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;
	m_WeaponAmmoSystem.OnInit(pd3dDevice, hWnd);

	return true;
}

bool Weapon::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd,
	int Damage, int WeaponMagazine)
{
	OnInit(pd3dDevice, hWnd);
	//m_WeaponModel.OnInit(ModelPath, ModelTexture, m_pd3dDevice, );

	m_WeaponDamage = Damage;
	m_WeaponMagazine = WeaponMagazine;
	m_WeaponAmmoSystem.OnInit(m_pd3dDevice, m_hWnd, m_WeaponMagazine);
	return true;
}

bool Weapon::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd,
	const char* ShootSoundPath,
	const char* ReloadSoundPath,
	const char* OringSoundPath,
	int Damage, int WeaponMagazine)
{
	OnInit(pd3dDevice, hWnd, Damage, WeaponMagazine);
	m_ReloadSound.OnInit(hWnd, ReloadSoundPath);
	m_ShootSound.OnInit(hWnd, ShootSoundPath);
	m_OringSound.OnInit(hWnd, OringSoundPath);
	return true;
}

bool Weapon::OnRender()
{
	m_WeaponAmmoSystem.OnRender();
	return true;
}

bool Weapon::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_WeaponAmmoSystem.OnUpdate();
	return true;
}

bool Weapon::OnRelease()
{
	m_WeaponAmmoSystem.OnRelease();
	m_ReloadSound.OnRelease();
	m_ShootSound.OnRelease();
	m_OringSound.OnRelease();
	return true;
}

bool Weapon::Load(float SoundX, float SoundY, float SoundZ)
{
	return true;
}

bool Weapon::Reload(float SoundX, float SoundY, float SoundZ)
{
	if (!m_WeaponAmmoSystem.ReloadAmmo())
		return false;
	m_ReloadSound.PlayWaveFile();
	return true;
}

bool Weapon::Shoot(float SoundX, float SoundY, float SoundZ)
{
	if (!m_WeaponAmmoSystem.ShootAmmo())
	{
		m_OringSound.PlayWaveFile();
		return false;
	}
	m_ShootSound.PlayWaveFile();

	return true;
}

void Weapon::SetDamage(int Damage)
{
	m_WeaponDamage = Damage;
}

void Weapon::SetMagazine(int Magazine)
{
	m_WeaponMagazine = Magazine;
}

void Weapon::SetLeftAmmo(int LeftAmmo)
{
	m_WeaponAmmoSystem.SetAmmoLeft(LeftAmmo);
}

void Weapon::SetLoadAmmo(int LoadAmmo)
{
	if (LoadAmmo > m_WeaponMagazine)
		LoadAmmo = m_WeaponMagazine;
	m_WeaponAmmoSystem.SetAmmoLoaded(LoadAmmo);
}

int Weapon::GetLeftAmmo()
{
	return m_WeaponAmmoSystem.GetAmmoLeft();
}

int Weapon::GetLoadAmmo()
{
	return m_WeaponAmmoSystem.GetAmmoLoaded();
}

bool Weapon::ShootWeaponAnimate(float Time)
{
	return true;
}

bool Weapon::LoadWeaponAnimate(float Time)
{
	return true;
}

bool Weapon::ReloadAnimate(float Time)
{
	return true;
}

int Weapon::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
int Weapon::HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void Weapon::SetListener(float x, float y, float z)
{
	//m_OringSound.SetListener(x, y, z);
	//m_ShootSound.SetListener(x, y, z);
	//m_ReloadSound.SetListener(x, y, z);
}