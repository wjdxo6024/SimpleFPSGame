#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9anim.h>
#include "AllocateHierachy.h"

class AniMesh
{
public:
	AniMesh();
	~AniMesh();
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName);
	void OnRender();
	void OnUpdate(float fElapsedTime);
	void OnRelease();
public:
	HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame);
	HRESULT UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DTEXTURE9 m_Texture;

	AllocateHierarchy m_alloc;
	LPD3DXFRAME m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
};