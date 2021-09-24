#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	FLOAT tu, tv;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

class Sphere
{
public:
	Sphere();
	~Sphere();

	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, float Rad , float X, float Y, float Z);
	void OnRender();
	void OnUpdate();
	void OnRelease();
	float GetRad();
	D3DXVECTOR3 GetCenter();

private:
	D3DMATERIAL9 mtrl;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXMESH m_pSphereMesh;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 m_pTexture;

	float m_curX;
	float m_curY;
	float m_curZ;
	float m_Rad;

	int m_Degree;
};

