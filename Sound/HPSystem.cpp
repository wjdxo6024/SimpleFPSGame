#include "HPSystem.h"

bool HPSystem::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;

	SetHP(100);

	RECT rt;
	GetClientRect(m_hWnd, &rt);
	m_HPText.OnInit(m_pd3dDevice, (rt.bottom / 30) + (rt.right / 30));
	m_OutputTextPosX = rt.right / 24;
	m_OutputTextPosY = rt.bottom - (rt.bottom / 24 * 2);

	return true;
}

bool HPSystem::OnRender()
{
	wchar_t HPText[8];
	wsprintf(HPText, L"HP: %d", m_PlayerHP);
	m_HPText.OnRender(m_OutputTextPosX, m_OutputTextPosY, HPText, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	return true;
}

// 그저 반환만 하는 메소드
bool HPSystem::OnUpdate()
{
	return true;
}

// 만약 죽을 경우 false를 반환한다.
bool HPSystem::OnUpdate(bool IsDamage, int Damage)
{
	if (IsDamage)
	{
		m_PlayerHP -= Damage;
		if (m_PlayerHP < 0)
		{
			m_PlayerHP = 0;
			return false;
		}
	}
	return true;
}

bool HPSystem::OnRelease()
{
	m_HPText.OnRelease();

	return true;
}

int HPSystem::GetHP()
{
	return m_PlayerHP;
}

void HPSystem::SetHP(int hp)
{
	m_PlayerHP = hp;
}