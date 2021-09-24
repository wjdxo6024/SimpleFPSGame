#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include "BoundingSphere.h"
#include "BoundingBox.h"

struct Ray
{
	D3DXVECTOR3 m_origin;
	D3DXVECTOR3 m_direction;
};

Ray CalcPickingRay(int x, int y, LPDIRECT3DDEVICE9 pd3dDevice);

void TransformRay(Ray* pRay, D3DXMATRIX* T);

bool raySphereIntersectionTest(Ray* ray, BoundingSphere* sphere);

bool rayBoxIntersectionTest(Ray* ray, BoundingBox* box);