#include "MultiAnimStructs.h"

HRESULT MultiAnimMC::SetupBonePtrs(D3DXFRAME* pFrameRoot)
{
	if (pSkinInfo)
	{
		if (m_apmxBonePointers)
			delete[] m_apmxBonePointers;

		DWORD dwNumBones = pSkinInfo->GetNumBones();

		m_apmxBonePointers = new D3DXMATRIX*[dwNumBones];
		if (m_apmxBonePointers == NULL)
			return E_OUTOFMEMORY;

		for (DWORD i = 0; i < dwNumBones; ++i)
		{
			MultiAnimFrame* pFrame = (MultiAnimFrame*)D3DXFrameFind(pFrameRoot, pSkinInfo->GetBoneName(i));
			if (pFrame == NULL)
				return E_FAIL;

			m_apmxBonePointers[i] = &pFrame->TransformationMatrix;
		}
	}

	return S_OK;
}
