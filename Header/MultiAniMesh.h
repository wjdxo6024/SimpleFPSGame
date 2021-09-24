#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "MultiAnimManager.h"
#include "MultiAnimStructs.h"
#include "MultiAllocateHierarchy.h"

class MultiAniMesh
{
public:
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnRender();
	void OnUpdate();
	void OnRelease();

	MultiAniMesh();
	~MultiAniMesh();

	void GetPosition();
	void GetFacing();

	void SetPosition(double x, double y, double z);
	void SetFacing(double x, double y, double z);

	HRESULT ResetTime();
	HRESULT AdvanceTime(double dTime);

private:
	void Animate();
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	MultiAnimManager m_pMultiAnimManager;
	MultiAllocateHierarchy* m_pAllocate;
	
};

