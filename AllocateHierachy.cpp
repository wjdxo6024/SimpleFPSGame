#include "stdafx.h"
#include "AllocateHierachy.h"
//--------------------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//--------------------------------------------------------------------------------------
HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}

HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED* pFrame;

	*ppNewFrame = NULL;

	pFrame = new D3DXFRAME_DERIVED;
	if (pFrame == NULL)
	{
		delete pFrame;
		return E_FAIL;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
	{
		delete pFrame;
		return E_FAIL;
	}

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	return S_OK;
}

HRESULT AllocateHierarchy::CreateMeshContainer(LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		if (pd3dDevice != NULL)
		{
			pd3dDevice->Release();
			pd3dDevice = NULL;
		}

		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return E_FAIL;
	}

	pMesh = pMeshData->pMesh;

	if (pMesh->GetFVF() == 0)
	{
		if (pd3dDevice != NULL)
		{
			pd3dDevice->Release();
			pd3dDevice = NULL;
		}

		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return E_FAIL;
	}

	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	if (pMeshContainer == NULL)
	{
		if (pd3dDevice != NULL)
		{
			pd3dDevice->Release();
			pd3dDevice = NULL;
		}

		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return E_FAIL;
	}

	memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
	{
		if (pd3dDevice != NULL)
		{
			pd3dDevice->Release();
			pd3dDevice = NULL;
		}

		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return E_FAIL;
	}

	pMesh->GetDevice(&pd3dDevice);
	NumFaces = pMesh->GetNumFaces();

	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pMeshContainer->MeshData.pMesh);

		if (FAILED(hr))
		{
			if (pd3dDevice != NULL)
			{
				pd3dDevice->Release();
				pd3dDevice = NULL;
			}

			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return E_FAIL;
		}

		pMesh = pMeshContainer->MeshData.pMesh;

		D3DXComputeNormals(pMesh, NULL);
	}
	else
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	
		pMesh->AddRef();
	}

	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		if (pd3dDevice != NULL)
		{
			pd3dDevice->Release();
			pd3dDevice = NULL;
		}

		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return E_FAIL;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	// 
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				WCHAR strTexturePath[MAX_PATH];
				WCHAR wszBuf[MAX_PATH];
				MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[iMaterial].pTextureFilename, -1, wszBuf, MAX_PATH);
				wszBuf[MAX_PATH - 1] = L'\0';
				
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			if (pd3dDevice != NULL)
			{
				pd3dDevice->Release();
				pd3dDevice = NULL;
			}

			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return E_FAIL;
		}

		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}

		hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
		if (FAILED(hr))
		{
			if (pd3dDevice != NULL)
			{
				pd3dDevice->Release();
				pd3dDevice = NULL;
			}

			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return E_FAIL;
		}
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;
}

HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{

	if (pFrameToFree->Name != NULL)
	{
		delete[] pFrameToFree->Name;
		pFrameToFree->Name = NULL;
	}

	if (pFrameToFree != NULL)
	{
		delete pFrameToFree;
		pFrameToFree = NULL;
	}

	return S_OK;
}

HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{

	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	if (pMeshContainer->Name != NULL)
	{
		delete[] pMeshContainer->Name;
		pMeshContainer->Name = NULL;
	}

	if (pMeshContainer->pAdjacency != NULL)
	{
		delete[] pMeshContainer->pAdjacency;
		pMeshContainer->pAdjacency = NULL;
	}

	if (pMeshContainer->pMaterials != NULL)
	{
		delete[] pMeshContainer->pMaterials;
		pMeshContainer->pMaterials = NULL;
	}

	if (pMeshContainer->pBoneOffsetMatrices != NULL)
	{
		delete[] pMeshContainer->pBoneOffsetMatrices;
		pMeshContainer->pBoneOffsetMatrices = NULL;
	}

	// release all the allocated textures
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			if (pMeshContainer->ppTextures[iMaterial] != NULL)
			{
				pMeshContainer->ppTextures[iMaterial]->Release();
				pMeshContainer->ppTextures[iMaterial] = NULL;
			}
		}
	}

	if (pMeshContainer->ppTextures != NULL)
	{
		delete[] pMeshContainer->ppTextures;
		pMeshContainer->ppTextures = NULL;
	}

	if (pMeshContainer->ppBoneMatrixPtrs != NULL)
	{
		delete[] pMeshContainer->ppBoneMatrixPtrs;
		pMeshContainer->ppBoneMatrixPtrs = NULL;
	}

	if (pMeshContainer->pBoneCombinationBuf != NULL)
	{
		pMeshContainer->pBoneCombinationBuf->Release();
		pMeshContainer->pBoneCombinationBuf = NULL;
	}

	if (pMeshContainer->MeshData.pMesh != NULL)
	{
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = NULL;
	}

	if (pMeshContainer->pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo->Release();
		pMeshContainer->pSkinInfo = NULL;
	}

	if (pMeshContainer->pOrigMesh != NULL)
	{
		pMeshContainer->pOrigMesh->Release();
		pMeshContainer->pOrigMesh = NULL;
	}

	if (pMeshContainer != NULL)
	{
		delete pMeshContainer;
		pMeshContainer = NULL;
	}
	return S_OK;
}

HRESULT GenerateSkinnedMesh(IDirect3DDevice9*pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer)
{
	HRESULT hr = S_OK;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);

	if (pMeshContainer->pSkinInfo == NULL)
		return hr;

	bool bUseSoftwareVP = false;
	if (pMeshContainer->MeshData.pMesh != NULL)
	{
		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = NULL;
	}
	if (pMeshContainer->pBoneCombinationBuf != NULL)
	{
		pMeshContainer->pBoneCombinationBuf->Release();
		pMeshContainer->pBoneCombinationBuf = NULL;
	}
	hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
		pMeshContainer->pOrigMesh,
		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
		pMeshContainer->pAdjacency,
		NULL, NULL, NULL,
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups,
		&pMeshContainer->pBoneCombinationBuf,
		&pMeshContainer->MeshData.pMesh
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	LPD3DXBONECOMBINATION rgBoneCombinations = reinterpret_cast<LPD3DXBONECOMBINATION>(
		pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

	for (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups;
		pMeshContainer->iAttributeSW++)
	{
		DWORD clnfl = 0;

		for (DWORD ilnfl = 0; ilnfl < pMeshContainer->NumInfl; ilnfl++)
		{
			if (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId)
			{
				++clnfl;
			}
		}

		if (clnfl > d3dCaps.MaxVertexBlendMatrices)
		{
			break;
		}
	}

	if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
	{
		LPD3DXMESH pMeshTmp;

		hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING |
			pMeshContainer->MeshData.pMesh->GetOptions(),
			pMeshContainer->MeshData.pMesh->GetFVF(),
			pd3dDevice, &pMeshTmp);

		if (FAILED(hr))
		{
			return E_FAIL;
		}

		pMeshContainer->MeshData.pMesh->Release();
		pMeshContainer->MeshData.pMesh = pMeshTmp;
		pMeshTmp = NULL;
	}
}


void DrawMeshContainer(IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	UINT iPaletteEntry;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer
		());

		// Draw using default vtx processing of the device (typically HW)
		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			NumBlend = 0;
			for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
			{
				if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
				{
					NumBlend = i;
				}
			}

			if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
			{
				// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
				for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
						pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
					}
				}

				pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

				// lookup the material used for this subset of faces
				if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
				{
					pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
					pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}

				// draw the subset now that the correct material and matrices are loaded
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}

		// If necessary, draw parts that HW could not handle using SW
		if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
		{
			AttribIdPrev = UNUSED32;
			pd3dDevice->SetSoftwareVertexProcessing(TRUE);
			for (iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				NumBlend = 0;
				for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
				{
					if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
					{
						NumBlend = i;
					}
				}

				if (d3dCaps.MaxVertexBlendMatrices < NumBlend + 1)
				{
					// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
					for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
					{
						iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
						if (iMatrixIndex != UINT_MAX)
						{
							D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
								pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
							pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
						}
					}

					pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

					// lookup the material used for this subset of faces
					if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
					{
						pd3dDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
						pd3dDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
						AttribIdPrev = pBoneComb[iAttrib].AttribId;
					}

					// draw the subset now that the correct material and matrices are loaded
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
				}
			}
			pd3dDevice->SetSoftwareVertexProcessing(FALSE);
		}

		pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
	}
}

void DrawFrame(IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pd3dDevice, pMeshContainer, pFrame);

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pd3dDevice, pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pd3dDevice, pFrame->pFrameFirstChild);
	}
}