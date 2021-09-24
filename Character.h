#pragma once
#include "Object.h"

using namespace std;

enum Option { OnlyMeterial, WithTexture, WithShadow };

class Character :
	public Object
{

public:
	enum MovingState
	{
		Nothing = 0,
		Moving,
		Rotating,
		TestMoving
	};

	// �⺻ �Լ�
public:
	Character();
	~Character();

	void OnInit(const char* filename, const char* TexFileName, LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light, float x, float y, float z, float size_x, float size_y, float size_z);
	void OnInit(const char* filename,  LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light, float x, float y, float z, float size_x, float size_y, float size_z);
	void OnRender(Camera& camera);

	void OnUpdate();
	void OnUpdate(float x, float y, float z);
	void OnUpdate(int angle);

	void OnRelease();
	
	D3DXVECTOR3 GetPos();

	void ShowInformation(LPD3DXFONT Font);
	// ĳ���� �ΰ��� �Լ�
private:
	void Rotate(int angle);
	void RotateX();
	void RotateY();

	void MoveX(float speed = 0.01f);
	void MoveZ(float speed = 0.01f);
	void MoveY(float speed = 0.01f);

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	int iControlPointIndex = 0;
	vector<ModelVertex> m_Vertices;
	vector<DWORD> m_Indices;

	int triCount = 0;
	int VertexCount;

	int* Indices = 0;

	// �� ������ ����
	MovingState CharacterMovingState;

	// �� ��ǥ
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

	LPD3DXEFFECT m_ToonShader;
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

	// �ɼ�
	Option fxOption;

	// ��ǥ
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;

	D3DXMATRIXA16 matTrans;

	D3DXMATRIXA16 matRotationX;
	D3DXMATRIXA16 matRotationY;

	D3DXVECTOR3 m_Direction;
	D3DXVECTOR3 m_PosTarget;
	float velocity;

	float Angle;

	// ���� �ڽ�
	BoundingBox m_boundingBox;

	// ī�޶�
	Camera m_Camera;
};

