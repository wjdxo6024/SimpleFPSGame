#pragma once
#include "Object.h"

struct SKINNINGVERTEX
{
	D3DXVECTOR3 position;
	FLOAT b[3];
	DWORD index;
	DWORD  color;
	FLOAT tu, tv;
};

#define D3DFVF_SKINNINGVERTEX (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct SKINNINGINDEX
{
	WORD _0, _1, _2;
};

class Skinning :
	public Object
{
public:
	Skinning();
	~Skinning();
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnRender();
	void OnUpdate();
	void OnRelease();

private:
	void Animate();
	void DrawMesh();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXMATRIXA16 m_mat0;
	D3DXMATRIXA16 m_mat1;
};

