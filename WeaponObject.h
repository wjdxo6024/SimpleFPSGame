#pragma once
#include "Object.h"
class WeaponObject :
	public Object
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	int iControlPointIndex = 0;
	std::vector<ModelVertex> m_Vertices;
	std::vector<DWORD> m_Indices;

	// 모델 좌표
	D3DXMATRIXA16 matWorld;

	float m_x;
	float m_y;
	float m_z;

	float m_sizeX;
	float m_sizeY;
	float m_sizeZ;

	// 라이트
	D3DLIGHT9* pLight;

	//머터리얼
	D3DMATERIAL9 mtrl;
	D3DMATERIAL9 ShadowMtrl;

	//셰이더
	LPD3DXEFFECT m_ModelEffect;

	LPD3DXEFFECT m_ApplyShadowShader;
	LPD3DXEFFECT m_CreateShadowShader;

	//텍스쳐
	LPDIRECT3DTEXTURE9 m_ModelTexture;

	// 그림자
	LPDIRECT3DTEXTURE9 m_ShadowRenderTarget = NULL;
	LPDIRECT3DSURFACE9 m_ShadowDepthStencil = NULL;


	//모델
	LPD3DXMESH Model;

	//버텍스 버퍼
	LPDIRECT3DVERTEXBUFFER9 pVB;
	LPDIRECT3DINDEXBUFFER9 pIB;

	// test
	int Rotation = 0;

public:
	void OnInit(const char* filename, const char* TexFileName, LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light, float x, float y, float z, float size_x, float size_y, float size_z);
	void OnRender(Camera& camera);
	void OnUpdate();
	void OnRelease();

	void MoveX(float distance);
	void MoveY(float distance);
	void MoveZ(float distance);
};

