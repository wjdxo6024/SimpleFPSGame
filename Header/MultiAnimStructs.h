#pragma once
#ifndef _MULTIANIMSTRUCTS_H_
#define _MULTIANIMSTRUCTS_H_
#include <d3d9.h>
#include <d3dx9.h>

struct MultiAnimFrame : public D3DXFRAME
{
};

struct MultiAnimMC : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* m_apTextures;
	LPD3DXMESH m_pWorkingMesh;
	D3DXMATRIX* m_amxBoneOffsets;  // Bone offset matrices retrieved from pSkinInfo
	D3DXMATRIX** m_apmxBonePointers;  // Provides index to bone matrix lookup

	DWORD m_dwNumPaletteEntries;
	DWORD m_dwMaxNumFaceInfls;
	DWORD m_dwNumAttrGroups;
	LPD3DXBUFFER m_pBufBoneCombos;

	HRESULT SetupBonePtrs(D3DXFRAME* pFrameRoot);
};
#endif