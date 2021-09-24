#pragma once

#ifndef _MULTIANIMATION_H_
#define _MULTIANIMATION_H_
#include <vector>

#include <d3d9.h>
#include <d3dx9.h>

#include "MultiAnimManager.h"
#include "AnimInstance.h"
#include "MultiAnimStructs.h"

// class CMultiAnim; = Manager
// class CAnimInstance; = AnimInstance

class MultiAnimManager;
class AnimInstance;

class MultiAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	MultiAllocateHierarchy();
	~MultiAllocateHierarchy();

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	STDMETHOD(SetMA)(THIS_ MultiAnimManager *pMA);

private:
	MultiAnimManager* m_pMA;
};


#endif
