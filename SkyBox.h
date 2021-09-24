#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include "Camera.h"

class SkyBox
{
public:

	SkyBox();
	~SkyBox();

	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera);
	void OnRender(Camera& camera);
	void OnUpdate();
	void OnRelease();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;

	LPDIRECT3DCUBETEXTURE9 m_SkyTexture;
	LPD3DXEFFECT m_SkyBoxEffect;
	LPD3DXMESH m_SkyBoxMesh;
};