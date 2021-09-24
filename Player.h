#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>
#include "Camera.h"
#include "BoundingBox.h"
#include "FBXLoad.h"
#include "MyD3DUtil.h"

// Fps System
#include "FPSInterface.h"

class Player
{
protected:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DTEXTURE9 m_pTexture[4];
	LPDIRECT3DTEXTURE9 m_TestTexture;
	D3DMATERIAL9 mtrl;

	LPD3DXEFFECT m_ModelEffect;

	// 모델
	std::vector<ModelVertex> m_Vertices;
	std::vector<DWORD> m_Indices;

	int triCount = 0;
	int VertexCount;

	//버텍스 버퍼
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;


	// 게임 시스템
	int Team; // Red = 0, Blue = 1
	int nLife; // 기본 100

	float m_fSpeed;

	DWORD dwBulletFireTime;
	DWORD dwOldBulletFireTime;

	float m_curX;
	float m_curY;
	float m_curZ;

	float m_sizeX;
	float m_sizeY;
	float m_sizeZ;

	float m_ColJud;

	bool is_GoLeft;
	bool is_GoRight;

	// Fps Interface
	FPSInterface m_PlayerFPSInterface;

	// 매트릭스
	D3DXMATRIXA16 m_matWorld;
	D3DXMATRIXA16 m_matView;
	D3DXMATRIXA16 m_matProj;
	D3DXMATRIXA16 m_matAni;

public:
	Camera* m_playerCamera;
	BoundingBox m_PlayerBox;
	BoundingBox m_PlayerCollisionBox;

private:
	bool HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	Player();
	~Player();

	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* texture, Camera* Camera, HWND hWnd);
	virtual void OnRender();
	virtual void OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnRelease();

	void MoveX();
	void MoveY();
	void MoveZ();

	Camera* GetCamera();

	D3DXVECTOR3 GetVec();
	float GetRad();

	int GetLife();
	DWORD GetFireTime();
	DWORD GetOldFireTime();
	void SetOldFireTime(DWORD nTime);
	void Damage();
	D3DXVECTOR3 GetPlayerPos();
};
