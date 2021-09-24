#pragma once
#include <d3d9.h>
#include <Windows.h>
#include <d3dx9math.h>
#include "MyDef.h"

#include <iostream>
#include <cstdlib>

LPD3DXMESH LoadModel(const char * filename, LPDIRECT3DDEVICE9 m_pd3dDevice);


LPDIRECT3DTEXTURE9 LoadTexture(const char* filename, LPDIRECT3DDEVICE9 m_pd3dDevice);


LPD3DXEFFECT LoadShader(const char * filename, LPDIRECT3DDEVICE9 m_pd3dDevice);

BOOL CheckSphereIntersect(D3DXVECTOR3 vCenter1, float fRadius1, D3DXVECTOR3 vCenter2,
	float fRadius2);

BOOL CheckCubeIntersection(
	D3DXVECTOR3* vMin1, D3DXVECTOR3* vMax1, D3DXVECTOR3* vMin2, D3DXVECTOR3* vMax2);

//template<typename T>
//T AbsoluteNum(T Num);

const float AbsoluteNum(float Num);

float GetRandomFloat(float lowBound, float highBound);

void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);

DWORD FtoDw(float f);

float Linear(float v0, float v1, float t);
