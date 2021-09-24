#pragma once
#include <d3dx9math.h>

struct BoundingSphere
{
	BoundingSphere();

	D3DXVECTOR3 m_center;
	float m_radius;
};