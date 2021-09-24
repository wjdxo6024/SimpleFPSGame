#pragma once
#include "Object.h"
#include <d3d9.h>
#include <d3dx9.h>

struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct MYINDEX
{
	WORD _0, _1, _2; // �Ϲ������� �ε����� 16��Ʈ�� ũ�⸦ ���´�.
};

class Hierarchy : public Object
{
public:
	Hierarchy();
	~Hierarchy();

	void OnInit(LPDIRECT3DDEVICE9  pd3dDevice);
	void OnRender();
	void OnUpdate();
	void OnRelease();

private:
	void Animate();
	void InitAnimate();
	void DrawMesh(D3DXMATRIXA16* pMat);

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXMATRIXA16 m_matTMParent;  // �θ��� TM
	D3DXMATRIXA16 m_matRParent; // �θ��� ȸ�����
	
	D3DXMATRIXA16 m_matTMChild; // �ڽ��� TM
	D3DXMATRIXA16 m_matRChild; // �ڽ��� ȸ�����

	D3DXVECTOR3 m_aniPos[2];
	D3DXQUATERNION m_aniRot[2];
};

