#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>

class Teapot
{
public:
	Teapot();
	~Teapot();
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnRender();
	void OnUpdate();
	void OnRelease();
private:
	void Animate();
	void DrawMesh();
private:

	D3DMATERIAL9 mtrl;
	LPDIRECT3DTEXTURE9 m_Texture;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXMESH m_Teapot;

	ID3DXAnimationController*   m_pAnimController = NULL;
};

