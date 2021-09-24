#include "stdafx.h"
#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}

void BoundingBox::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	D3DXCreateBox(m_pd3dDevice, 100, 200, 50, &m_pBoxMesh, NULL);
}

void BoundingBox::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR3& max, const D3DXVECTOR3& min, const D3DXVECTOR3& Pos, const D3DXVECTOR3& Size)
{
	m_pd3dDevice = pd3dDevice;
	D3DXVECTOR3 tmpMax, tmpMin, tmpPos;
	tmpMax = max;
	tmpMin = min;
	tmpPos = Pos;

	D3DXMatrixTranslation(&m_boxWorld, tmpPos.x, tmpPos.y, tmpPos.z);

	tmpMax += tmpPos;
	tmpMin += tmpPos;

	SetVector(tmpMax, tmpMin);
	D3DXCreateBox(m_pd3dDevice, m_max.x - m_min.x, m_max.y - m_min.y, m_max.z - m_min.z, &m_pBoxMesh, NULL);
}

void BoundingBox::OnRender(D3DXMATRIXA16& matWorld)
{
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_boxWorld *= matWorld;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_boxWorld);
	m_pBoxMesh->DrawSubset(0);

	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
}

// 1. max벡터와 min벡터의 위치가 현재 송출되고 있는 box의 max, min 벡터와 일치해야 함.
// 2. 
void BoundingBox::OnUpdate()
{
	
}

void BoundingBox::OnRelease()
{
	if (m_pBoxMesh != NULL)
		m_pBoxMesh->Release();
}


void BoundingBox::SetVector(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	m_max = max;
	m_min = min;
}

D3DXVECTOR3* BoundingBox::GetMax()
{
	return &m_max;
}

D3DXVECTOR3* BoundingBox::GetMin()
{
	return &m_min;
}

bool BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if (p.x >= m_min.x && p.y >= m_min.y && p.z >= m_min.z &&
		p.x <= m_max.x && p.y <= m_max.y && p.z <= m_max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BoundingBox::IsCollideBullet(Bullet* bullet)
{
	float tmin = (m_min.x - bullet->posX) / bullet->dirX;
	float tmax = (m_max.x - bullet->posX) / bullet->dirX;

	if (tmin > tmax)
	{
		// Swap
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}

	float tymin = (m_min.y - bullet->posY) / bullet->dirY;
	float tymax = (m_max.y - bullet->posY) / bullet->dirY;

	if (tymin > tymax)
	{
		// Swap
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (m_min.z - bullet->posZ) / bullet->dirZ;
	float tzmax = (m_max.z - bullet->posZ) / bullet->dirZ;

	if (tzmin > tzmax)
	{
		// Swap
		float temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

bool BoundingBox::isCollideBox(BoundingBox* otherBox)
{
	if (otherBox->GetMax()->x < m_min.x || otherBox->GetMin()->x > m_max.x) return true;
	if (otherBox->GetMax()->y < m_min.y || otherBox->GetMin()->y > m_max.y) return true;
	if (otherBox->GetMax()->z < m_min.z || otherBox->GetMin()->z > m_max.z) return true;

	return false;
}