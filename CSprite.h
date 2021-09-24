#pragma once
#include "MyDef.h"
#include <d3dx9core.h>
#include <filesystem>
#include <iostream>
// ��������Ʈ�� ���

class CSprite
{
private:
	LPD3DXSPRITE m_Sprite;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 position;
	D3DCOLOR color;
public:
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* path);
	void OnRender(int x, int y, int size);
	void OnUpdate();
	void OnRelease();

	// �̿ϼ� �޼���
	void OnRenderAnimatiion(int x, int y, int size, float fTime, float fElapsedTime);
};

