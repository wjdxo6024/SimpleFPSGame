#pragma once
#include <d3dx9math.h>
#include <d3dx9shape.h>
#include <d3d9.h>

class ColCube
{
public:
	ColCube();
	~ColCube();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
};

