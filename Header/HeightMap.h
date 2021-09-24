#pragma once
#include "Object.h"
#include "Frustum.h"
#include "Camera.h"

struct HMVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR3 t;
};

struct HMINDEX
{
	WORD _0, _1, _2;
};

#define D3DFVF_HMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

class HeightMap : public Object
{
public:
	HeightMap();
	~HeightMap();

	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, Camera* pCamera);
	void OnRender();
	void OnUpdate();
	void OnRelease();
	
	void FrustumOn();

private:
	void Animate();
	void DrawMesh(D3DXMATRIXA16* pMat);
	HRESULT ProcessFrustumCull();
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPDIRECT3DTEXTURE9 m_pTexHeight;
	LPDIRECT3DTEXTURE9 m_pTexDiffuse;

	D3DXMATRIXA16 m_matAni;

	DWORD m_cxHeight;
	DWORD m_czHeight;

	Camera* m_Camera;

	int m_nTriangles;
	D3DXVECTOR3* m_pvHeightMap = NULL;
	Frustum* m_pFrustum;
	BOOL m_bHideFrustum = TRUE;
	BOOL m_bLockFrustum = FALSE;
};

