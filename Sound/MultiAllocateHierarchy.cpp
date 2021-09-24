#include "stdafx.h"
#include "MultiAllocateHierarchy.h"


MultiAllocateHierarchy::MultiAllocateHierarchy()
{
}


MultiAllocateHierarchy::~MultiAllocateHierarchy()
{
}

CHAR* HeapCopy(CHAR* sName)
{
	DWORD dwLen = (DWORD)strlen(sName) + 1;
	CHAR* sNewName = new CHAR[dwLen];
	if (sNewName)
		strcpy_s(sNewName, dwLen, sName);
	return sNewName;
}

HRESULT MultiAllocateHierarchy::SetMA(THIS_ MultiAnimManager *pMA)
{
	m_pMA = pMA;

	return S_OK;
}

HRESULT MultiAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME* ppFrame)
{

	*ppFrame = NULL;

	MultiAnimFrame * pFrame = new MultiAnimFrame;
	if (pFrame == NULL)
	{
		DestroyFrame(pFrame);
		MessageBox(NULL, L"Cannot make Frame, Out of Memory.", L"Error", MB_OK);
		return E_OUTOFMEMORY;
	}

	ZeroMemory(pFrame, sizeof(MultiAnimFrame));

	if (Name)
	{
		pFrame->Name = (CHAR*)HeapCopy((CHAR*)Name);
	}
	else
	{
		// TODO: Add a counter to append to the string below
   //       so that we are using a different name for
   //       each bone.
		pFrame->Name = (CHAR*)HeapCopy((CHAR*)"<no_name>");
	}

	if (pFrame->Name == NULL)
	{
		DestroyFrame(pFrame);
		MessageBox(NULL, L"Cannot make Frame Name, Out of Memory.", L"Error", MB_OK);
		return E_OUTOFMEMORY;
	}

	*ppFrame = pFrame;
	return S_OK;
}

HRESULT MultiAllocateHierarchy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	*ppNewMeshContainer = NULL;

	HRESULT hr = S_OK;

	MultiAnimMC * pMC = new MultiAnimMC;
	if (pMC == NULL)
	{
		MessageBox(NULL, L"Cannot make MeshContainer. Out of Memory.", L"Error", MB_OK);
		hr = E_OUTOFMEMORY;
		
		if (FAILED(hr))
		{
			if (pMC)
				DestroyMeshContainer(pMC);
		}
		else
			*ppNewMeshContainer = pMC;

		return hr;
	}

	ZeroMemory(pMC, sizeof(MultiAnimMC));

	if (pSkinInfo == NULL) // 스태틱 메쉬일 경우, 빠르게 종료
	{
		hr = S_OK;

		if (FAILED(hr))
		{
			if (pMC)
				DestroyMeshContainer(pMC);
		}
		else
			*ppNewMeshContainer = pMC;

		return hr;
	}

	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		MessageBox(NULL, L"Just Support D3DXMESHTYPE_MESH.", L"Error", MB_OK);

		hr = E_FAIL;

		if (FAILED(hr))
		{
			if (pMC)
				DestroyMeshContainer(pMC);
		}
		else
			*ppNewMeshContainer = pMC;

		return hr;
	}

	if (Name)
		pMC->Name = (CHAR*)HeapCopy((CHAR*)Name);
	else
		pMC->Name = (CHAR*)HeapCopy((CHAR*)"<no_name>");

	pMC->MeshData.Type = pMeshData->Type;
	pMC->MeshData.pMesh = pMeshData->pMesh;
	pMC->MeshData.pMesh->AddRef();

	{
		DWORD dwNumFaces = pMC->MeshData.pMesh->GetNumFaces();
		pMC->pAdjacency = new DWORD[3 * dwNumFaces];
		if (pMC->pAdjacency == NULL)
		{
			MessageBox(NULL, L"Cannot make pAdjacency. Out of Memory.", L"Error", MB_OK);
			hr = E_OUTOFMEMORY;

			if (FAILED(hr))
			{
				if (pMC)
					DestroyMeshContainer(pMC);
			}
			else
				*ppNewMeshContainer = pMC;

			return hr;
		}

		CopyMemory(pMC->pAdjacency, pAdjacency, 3 * sizeof(DWORD)*dwNumFaces);
	}

	pMC->pEffects = NULL;

	pMC->NumMaterials = max(1, NumMaterials);
	pMC->pMaterials = new D3DXMATERIAL[pMC->NumMaterials];
	pMC->m_apTextures = new LPDIRECT3DTEXTURE9[pMC->NumMaterials];
	if (pMC->pMaterials == NULL || pMC->m_apTextures == NULL)
	{
		MessageBox(NULL, L"Cannot make pMaterials and m_apTextures. Out of Memory.", L"Error", MB_OK);
		hr = E_OUTOFMEMORY;

		if (FAILED(hr))
		{
			if (pMC)
				DestroyMeshContainer(pMC);
		}
		else
			*ppNewMeshContainer = pMC;

		return hr;
	}

	if (NumMaterials > 0)
	{
		CopyMemory(pMC->pMaterials, pMaterials, NumMaterials * sizeof(D3DXMATERIAL));
		for (DWORD i = 0; i < NumMaterials; ++i)
		{
			if (pMC->pMaterials[i].pTextureFilename)
			{
				WCHAR sNewPath[MAX_PATH];
				WCHAR wszTexName[MAX_PATH];
				if (MultiByteToWideChar(CP_ACP, 0, pMC->pMaterials[i].pTextureFilename, -1, wszTexName, MAX_PATH) == 0)
					pMC->m_apTextures[i] = NULL;
				else
				{
					//TODO : 여기 부분에 텍스쳐 불러오는 코드 추가할 것
				}
			}
			else
				pMC->m_apTextures[i] = NULL;
		}
	
	}
	else
	{
		ZeroMemory(&pMC->pMaterials[0].MatD3D, sizeof(D3DMATERIAL9));
		pMC->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMC->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMC->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMC->pMaterials[0].MatD3D.Specular = pMC->pMaterials[0].MatD3D.Diffuse;
		pMC->pMaterials[0].pTextureFilename = NULL;
	}

	pMC->pSkinInfo = pSkinInfo;
	pSkinInfo->AddRef();

	pMC->m_amxBoneOffsets = new D3DXMATRIX[pSkinInfo->GetNumBones()];

	if (pMC->m_amxBoneOffsets == NULL)
	{
		MessageBox(NULL, L"Cannot make m_amxBoneOffests. Out of Memory.", L"Error", MB_OK);
		hr = E_OUTOFMEMORY;

		if (FAILED(hr))
		{
			if (pMC)
				DestroyMeshContainer(pMC);
		}
		else
			*ppNewMeshContainer = pMC;

		return hr;
	}
	{
		for (DWORD i = 0; i < pSkinInfo->GetNumBones(); ++i)
		{
			pMC->m_amxBoneOffsets[i] = *(D3DXMATRIX *)pSkinInfo->GetBoneOffsetMatrix(i);
		}
	}

	{
		UINT iPaletteSize = 0;
		m_pMA->m_pEffect->GetInt("MARIX_PALETTE_SIZE", (INT*)&iPaletteSize);
		pMC->m_dwNumPaletteEntries = min(iPaletteSize, pMC->pSkinInfo->GetNumBones());
	}

	hr = pMC->pSkinInfo->ConvertToIndexedBlendedMesh(pMC->MeshData.pMesh,
		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
		pMC->m_dwNumPaletteEntries,
		pMC->pAdjacency,
		NULL,
		NULL,
		NULL,
		&pMC->m_dwMaxNumFaceInfls,
		&pMC->m_dwNumAttrGroups,
		&pMC->m_pBufBoneCombos,
		&pMC->m_pWorkingMesh);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Cannot Convert Mesh.", L"Error", MB_OK);

		if (pMC)
				DestroyMeshContainer(pMC);
		else
			*ppNewMeshContainer = pMC;

		return hr;
	}

	if (m_pMA->m_dwWorkingPaletteSize < pMC->m_dwNumPaletteEntries)
	{
		if (m_pMA->m_amxWorkingPalette)
			delete[] m_pMA->m_amxWorkingPalette;


		m_pMA->m_dwWorkingPaletteSize = pMC->m_dwNumPaletteEntries;
		m_pMA->m_amxWorkingPalette = new D3DXMATRIX[m_pMA->m_dwWorkingPaletteSize];
		if (m_pMA->m_amxWorkingPalette == NULL)
		{
			m_pMA->m_dwWorkingPaletteSize = 0;
			hr = E_OUTOFMEMORY;
			MessageBox(NULL, L"Cannot make WorkingPalette. Out of Memory.", L"Error", MB_OK);

			if (FAILED(hr))
			{
				if (pMC)
					DestroyMeshContainer(pMC);
			}
			else
				*ppNewMeshContainer = pMC;

			return hr;
		}
	
	}

	DWORD dwOldFVF = pMC->m_pWorkingMesh->GetFVF();
	DWORD dwNewFVF = (dwOldFVF & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (dwNewFVF != dwOldFVF)
	{
		LPD3DXMESH pMesh;
		hr = pMC->m_pWorkingMesh->CloneMeshFVF(pMC->m_pWorkingMesh->GetOptions(),
			dwNewFVF,
			m_pMA->m_pd3dDevice,
			&pMesh);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"CloneMeshFVF Error", L"Error", MB_OK);

			if (pMC)
					DestroyMeshContainer(pMC);
			else
				*ppNewMeshContainer = pMC;

			return hr;
		}

		pMC->m_pWorkingMesh->Release();
		pMC->m_pWorkingMesh = pMesh;

		if (!(dwOldFVF & D3DFVF_NORMAL))
		{
			hr = D3DXComputeNormals(pMC->m_pWorkingMesh, NULL);
			if (FAILED(hr))
			{
				MessageBox(NULL, L"Cannot Compute Normals.", L"Error", MB_OK);
				if (pMC)
						DestroyMeshContainer(pMC);
				else
					*ppNewMeshContainer = pMC;

				return hr;
			}
		}
	}

	// MultiAnimation 예제 중 AllocHierarchy.cpp 303~ 320 부분은 뺌.

	return hr;
}

HRESULT MultiAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	MultiAnimFrame * pFrame = (MultiAnimFrame *)pFrameToFree;

	if (pFrame->Name)
		delete[] pFrame->Name;

	delete pFrame;

	return S_OK;
}

HRESULT MultiAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	MultiAnimMC * pMC = (MultiAnimMC *)pMeshContainerToFree;

	if (pMC->Name)
		delete[] pMC->Name;

	if (pMC->MeshData.pMesh)
		pMC->MeshData.pMesh->Release();

	if (pMC->pAdjacency)
		delete[] pMC->pAdjacency;

	if (pMC->pMaterials)
		delete[] pMC->pMaterials;

	for (DWORD i = 0; i < pMC->NumMaterials; ++i)
	{
		if (pMC->m_apTextures[i])
			pMC->m_apTextures[i]->Release();
	}

	if (pMC->m_apTextures)
		delete[] pMC->m_apTextures;

	if (pMC->pSkinInfo)
		pMC->pSkinInfo->Release();

	if (pMC->m_amxBoneOffsets)
		delete[] pMC->m_amxBoneOffsets;

	if (pMC->m_pWorkingMesh)
	{
		pMC->m_pWorkingMesh->Release();
		pMC->m_pWorkingMesh = NULL;
	}

	pMC->m_dwNumPaletteEntries = 0;
	pMC->m_dwMaxNumFaceInfls = 0;
	pMC->m_dwNumAttrGroups = 0;

	if (pMC->m_pBufBoneCombos)
	{
		pMC->m_pBufBoneCombos->Release();
		pMC->m_pBufBoneCombos = NULL;
	}

	if (pMC->m_apmxBonePointers)
		delete[] pMC->m_apmxBonePointers;

	delete pMeshContainerToFree;

	return S_OK;
}