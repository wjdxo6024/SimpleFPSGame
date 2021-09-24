#include "InitScene.h"

bool InitScene::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;
	m_Text.OnInit(m_pd3dDevice, 50);
	m_Sprite.OnInit(m_pd3dDevice, L"./Sprite/01.png");
	return true;
}

bool InitScene::OnRender()
{
	RECT rt;
	GetClientRect(m_hWnd, &rt);

	const char string[] = "Now Loading...";
	m_Text.OnRender(rt.right / 2 - (strlen(string) * 12), rt.bottom / 2, string, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_Sprite.OnRender(rt.right / 2 - 64, rt.bottom / 2 - 128, 1);
	return true;
}

bool InitScene::OnUpdate()
{
	return true;
}

bool InitScene::OnRelease()
{
	m_Text.OnRelease();
	m_Sprite.OnRelease();
	return true;
}