#pragma once
#include <d3dx9math.h>
#include <d3dx9shape.h>
#include "Bullet.h"
class BoundingBox
{
public:
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR3& max, const D3DXVECTOR3& min, const D3DXVECTOR3& Pos, const D3DXVECTOR3& Size);
	void OnRender(D3DXMATRIXA16& matWorld);
	void OnUpdate();
	void OnRelease();

	BoundingBox();
	~BoundingBox();
	void SetVector(D3DXVECTOR3 max, D3DXVECTOR3 min);
	D3DXVECTOR3* GetMax();
	D3DXVECTOR3* GetMin();
	bool isPointInside(D3DXVECTOR3& p);
	bool IsCollideBullet(Bullet* bullet);
	bool isCollideBox(BoundingBox* otherBox);
private:
	D3DXVECTOR3 m_max;
	D3DXVECTOR3 m_min;
	D3DXMATRIXA16 m_boxWorld;

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXMESH m_pBoxMesh;
};

