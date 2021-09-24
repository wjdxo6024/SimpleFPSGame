#pragma once
#include "MyDef.h"
#include <Windows.h>
#include <fbxsdk.h>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

void LoadAnimation();

void LoadUVInformation(FbxMesh* pMesh, std::vector<ModelVertex>& pOutVertexVector);

void LoadFBX(const char* filename, std::pair<int, int>& memberCounts, std::vector<ModelVertex>& Vertices, std::vector<DWORD>& Indices);

HRESULT LoadFBXwithAnimation(const char* filename,
	DWORD MeshOptions,
	LPDIRECT3DDEVICE9 pDevice,
	LPD3DXALLOCATEHIERARCHY pAlloc,
	LPD3DXLOADUSERDATA pUserDataLoader,
	LPD3DXFRAME* ppFrameHierarchy,
	LPD3DXANIMATIONCONTROLLER* ppAnimController);