#include "stdafx.h"
#include "Picking.h"

Ray CalcPickingRay(int x, int y, LPDIRECT3DDEVICE9 pd3dDevice)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	pd3dDevice->GetViewport(&vp);

	D3DXMATRIX proj;
	pd3dDevice->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray.m_origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray.m_direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
}


void TransformRay(Ray* pRay, D3DXMATRIX* T)
{
	// 광선의 원점을 변환한다. w = 1.
	D3DXVec3TransformCoord(
		&pRay->m_origin,
		&pRay->m_origin,
		T);

	// 광선의 방향을 변환한다. w = 0.
	D3DXVec3TransformNormal(
		&pRay->m_direction,
		&pRay->m_direction,
		T);

	// 방향을 정규화한다.
	D3DXVec3Normalize(&pRay->m_direction, &pRay->m_direction);
}

bool raySphereIntersectionTest(Ray* ray, BoundingSphere* sphere)
{
	D3DXVECTOR3 v = ray->m_origin - sphere->m_center;

	float b = 2.0f * D3DXVec3Dot(&ray->m_direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->m_radius * sphere->m_radius);

	float discriminant = (b*b) - (4.0f * c);

	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;
	return false;
}