#pragma once
#include "MyDef.h"
#include "Camera.h"
#include "FBXLoad.h"
#include "MyD3DUtil.h"
#include "BoundingBox.h"
#include <d3d9.h>
#include <d3dx9math.h>
#include <fbxsdk.h>
#include <vector>
#include <list>

class Object
{
public:
	Object();
	~Object();

	virtual void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light);
	virtual void OnRender(Camera& camera);
	virtual void OnUpdate();
	virtual void OnRelease();
	virtual std::pair<BOOL, float> IntersectRay(D3DXVECTOR3* Ray);

protected:
	LPDIRECT3DDEVICE9 m_pd3dDevice;

};

