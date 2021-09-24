#pragma once
#include "Object.h"

class Building :
	public Object
{
public:

	Building();
	~Building();

	void OnInit(const char* filename, const char* TexFileName, LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light, float x, float y, float z, float size_x, float size_y, float size_z);
	void OnRender(Camera& camera);
	void OnUpdate();
	void OnRelease();

	std::pair<BOOL, D3DXVECTOR3> IntersectRay(D3DXVECTOR3* Ray, D3DXVECTOR3* Origin);
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	int iControlPointIndex = 0;
	std::vector<ModelVertex> m_Vertices;
	std::vector<DWORD> m_Indices;

	// �׽�Ʈ ��
	std::vector<ModelVertex> m_TestVertices;

	int triCount = 0;
	int VertexCount;

	int* Indices = 0;

	// �� ��ǥ
	D3DXMATRIXA16 matWorld;

	float m_x;
	float m_y;
	float m_z;

	float m_sizeX;
	float m_sizeY;
	float m_sizeZ;

	// ����Ʈ
	D3DLIGHT9* pLight;

	//���͸���
	D3DMATERIAL9 mtrl;
	D3DMATERIAL9 ShadowMtrl;

	//���̴�
	LPD3DXEFFECT m_ModelEffect;

	LPD3DXEFFECT m_ApplyShadowShader;
	LPD3DXEFFECT m_CreateShadowShader;

	//�ؽ���
	LPDIRECT3DTEXTURE9 m_ModelTexture;

	// �׸���
	LPDIRECT3DTEXTURE9 m_ShadowRenderTarget = NULL;
	LPDIRECT3DSURFACE9 m_ShadowDepthStencil = NULL;


	//��
	LPD3DXMESH Model;

	//���ؽ� ����
	LPDIRECT3DVERTEXBUFFER9 pVB;
	LPDIRECT3DINDEXBUFFER9 pIB;
	
	// �ٿ�� �ڽ�
	
	// test
	int Rotation = 0;
};

