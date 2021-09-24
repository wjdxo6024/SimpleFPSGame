#include "stdafx.h"
#include "AniMesh.h"


AniMesh::AniMesh()
{
}


AniMesh::~AniMesh()
{
}

void AniMesh::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, char * szFileName)
{
	m_pd3dDevice = pd3dDevice;

	D3DXLoadMeshHierarchyFromXA(szFileName, D3DXMESH_MANAGED, m_pd3dDevice, &m_alloc, NULL,
		&m_pFrameRoot, &m_pAnimController);

	SetupBoneMatrixPointers(m_pFrameRoot);
}

void AniMesh::OnRender()
{
	DrawFrame(m_pd3dDevice, m_pFrameRoot);
}

void AniMesh::OnUpdate(float fElapsedTime)
{
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matTrans;

	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.4f);
	D3DXMatrixTranslation(&matTrans, 0.0f, 100.0f, 0.0f);
	matWorld = matScale * matTrans;
	// fElapsedTime / 1000정도면 매우 적당함.
	m_pAnimController->AdvanceTime(fElapsedTime / 1000, NULL);

	UpdateFrameMatrices(m_pFrameRoot, &matWorld);
}

void AniMesh::OnRelease()
{
	m_alloc.DestroyFrame(m_pFrameRoot);

	if (m_pAnimController != NULL)
	{
		m_pAnimController->Release();
		m_pAnimController = NULL;
	}
}

HRESULT AniMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iBone, cBones;
	D3DXFRAME_DERIVED* pFrame;

	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));

			if (pFrame == NULL)
				return E_FAIL;

			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;

		}
	}

	return S_OK;
}

HRESULT AniMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	HRESULT hr;
	if (pFrame->pMeshContainer != NULL)
	{
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

HRESULT AniMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}

	return S_OK;
}