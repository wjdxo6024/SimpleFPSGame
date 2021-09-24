#include "stdafx.h"
#include "AnimInstance.h"


AnimInstance::AnimInstance(MultiAnimManager* pMultiAnimManager)
{
}


AnimInstance::~AnimInstance()
{
}

HRESULT AnimInstance::Setup(LPD3DXANIMATIONCONTROLLER pAC)
{
	DWORD i, dwTracks;

	m_pAC = pAC;

	dwTracks = m_pAC->GetMaxNumTracks();
	for (i = 0; i < dwTracks; ++i)
		m_pAC->SetTrackEnable(i, FALSE);

	return S_OK;
}

void AnimInstance::UpdateFrames(MultiAnimFrame* pFrame, D3DXMATRIX* pmxBase)
{
	D3DXMatrixMultiply(&pFrame->TransformationMatrix,
		&pFrame->TransformationMatrix,
		pmxBase);

	if (pFrame->pFrameSibling)
		UpdateFrames((MultiAnimFrame*)pFrame->pFrameSibling,
			pmxBase);

	if (pFrame->pFrameFirstChild)
		UpdateFrames((MultiAnimFrame*)pFrame->pFrameFirstChild,
			&pFrame->TransformationMatrix);
}

void AnimInstance::DrawFrames(MultiAnimFrame* pFrame)
{
	if (pFrame->pMeshContainer)
		DrawMeshFrame(pFrame);

	if (pFrame->pFrameSibling)
		DrawFrames((MultiAnimFrame*)pFrame->pFrameSibling);

	if (pFrame->pFrameFirstChild)
		DrawFrames((MultiAnimFrame*)pFrame->pFrameFirstChild);
}

void AnimInstance::DrawMeshFrame(MultiAnimFrame*  pFrame)
{
	MultiAnimMC* pMC = (MultiAnimMC*)pFrame->pMeshContainer;
	D3DXMATRIX mx;

	if (pMC->pSkinInfo == NULL)
		return;

	LPD3DXBONECOMBINATION pBC = (LPD3DXBONECOMBINATION)(pMC->m_pBufBoneCombos->GetBufferPointer());
	DWORD dwAttrib, dwPalEntry;

	for (dwAttrib = 0; dwAttrib < pMC->m_dwNumAttrGroups; ++dwAttrib)
	{
		for (dwPalEntry = 0; dwPalEntry, pMC->m_dwNumPaletteEntries; ++dwPalEntry)
		{
			DWORD dwMatrixIndex = pBC[dwAttrib].BoneId[dwPalEntry];
			if (dwMatrixIndex != UINT_MAX)
				D3DXMatrixMultiply(&m_pMAnimManager->m_amxWorkingPalette[dwPalEntry],
					&(pMC->m_amxBoneOffsets[dwMatrixIndex]),
					pMC->m_apmxBonePointers[dwMatrixIndex]);
		}

		m_pMAnimManager->m_pEffect->SetMatrixArray("amPalette",
			m_pMAnimManager->m_amxWorkingPalette,
			pMC->m_dwNumPaletteEntries);

		m_pMAnimManager->m_pEffect->SetTexture("g_txScene", pMC->m_apTextures[pBC[dwAttrib].AttribId]);
	
		m_pMAnimManager->m_pEffect->SetInt("CurNumBones", pMC->m_dwMaxNumFaceInfls - 1);
	
		if (FAILED(m_pMAnimManager->m_pEffect->SetTechnique(m_pMAnimManager->m_sTechnique)))
		{
			return;
		}

		UINT uiPasses, uiPass;

		m_pMAnimManager->m_pEffect->Begin(&uiPasses, 0);
		for (uiPass = 0; uiPass < uiPasses; ++uiPass)
		{
			m_pMAnimManager->m_pEffect->BeginPass(uiPass);
			pMC->m_pWorkingMesh->DrawSubset(dwAttrib);
			m_pMAnimManager->m_pEffect->EndPass();
		}
		m_pMAnimManager->m_pEffect->End();
	}
}

void AnimInstance::Cleanup()
{
	if (m_pAC)
		m_pAC->Release();
}

MultiAnimManager* AnimInstance::GetMultiAnimManager()
{
	return m_pMAnimManager;
}

void AnimInstance::GetAnimController(LPD3DXANIMATIONCONTROLLER* pAC)
{
	m_pAC->AddRef();
	*pAC = m_pAC;
}

D3DXMATRIX AnimInstance::GetWorldTransform()
{
	return m_mxWorld;
}

void AnimInstance::SetWorldTransform(const D3DXMATRIX* pmxWorld)
{
	m_mxWorld = *pmxWorld;
}

HRESULT AnimInstance::AdvanceTime(DOUBLE dTimeDelta, ID3DXAnimationCallbackHandler* pCH)
{
	HRESULT hr;

	// apply all the animations to the bones in the frame hierarchy.
	hr = m_pAC->AdvanceTime(dTimeDelta, pCH);
	if (FAILED(hr))
		return hr;

	// apply the animations recursively through the hierarchy, and set the world.
	UpdateFrames(m_pMAnimManager->m_pFrameRoot, &m_mxWorld);

	return S_OK;
}

HRESULT AnimInstance::ResetTime()
{
	return m_pAC->ResetTime();
}

HRESULT AnimInstance::Draw()
{
	DrawFrames(m_pMAnimManager->m_pFrameRoot);

	return S_OK;
}