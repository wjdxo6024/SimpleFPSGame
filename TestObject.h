#pragma once
#include <d3d9.h>
#include <Windows.h>
#include <d3dx9math.h>
#include <vector>
#include "MyDef.h"
#include "Camera.h"

class TestObject
{

public:
	TestObject();
	~TestObject();

	virtual HRESULT OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

	void GetVertices(std::vector<ModelVertex> Vertices)
	{
		m_Vertices = Vertices;
	}

private:
	float sx;
	float sy;
	float sz;
	float mx;
	float my;
	float mz;
	std::vector<ModelVertex> m_Vertices;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 pVB;
};

