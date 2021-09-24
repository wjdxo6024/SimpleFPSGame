#include "stdafx.h"
#include "MultiAnimManager.h"


MultiAnimManager::MultiAnimManager() : m_pd3dDevice(NULL),
m_pEffect(NULL),
m_dwWorkingPaletteSize(0),
m_amxWorkingPalette(NULL),
m_pFrameRoot(NULL),
m_pAC(NULL)
{
}


MultiAnimManager::~MultiAnimManager()
{
	std::vector<AnimInstance*>::iterator itCur, itEnd = m_v_pAnimInstances.end();
	for (itCur = m_v_pAnimInstances.begin(); itCur != itEnd; ++itCur)
	{
		(*itCur)->Cleanup();
		delete *itCur;
	}

	m_v_pAnimInstances.clear();
}


HRESULT MultiAnimManager::CreateInstance(AnimInstance** ppAnimInstance)
{
	*ppAnimInstance = NULL;

	LPD3DXANIMATIONCONTROLLER pNewAC = NULL;
	HRESULT hr;
	AnimInstance* pAI = NULL;

	hr = m_pAC->CloneAnimationController(m_pAC->GetMaxNumAnimationOutputs(),
		m_pAC->GetMaxNumAnimationSets(),
		m_pAC->GetMaxNumTracks(),
		m_pAC->GetMaxNumEvents(),
		&pNewAC);

	if (SUCCEEDED(hr))
	{
		pAI = new AnimInstance(this);
		if (pAI == NULL)
		{
			hr = E_OUTOFMEMORY;
			if (FAILED(hr))
			{
				if (pAI)
					delete pAI;

				if (pNewAC)
					pNewAC->Release();
			}

			return hr;
		}

		hr = pAI->Setup(pNewAC);
		if (FAILED(hr))
		{
			if (pAI)
				delete pAI;

			if (pNewAC)
				pNewAC->Release();

			return hr;
		}

		*ppAnimInstance = pAI;
	}

	return hr;
}

HRESULT MultiAnimManager::SetupBonePtrs(MultiAnimFrame* pFrame)
{
	HRESULT hr;

	if (pFrame->pMeshContainer)
	{
		// call setup routine
		hr = ((MultiAnimMC*)pFrame->pMeshContainer)->SetupBonePtrs(m_pFrameRoot);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameSibling)
	{
		// recursive call
		hr = SetupBonePtrs((MultiAnimFrame*)pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameFirstChild)
	{
		// recursive call
		hr = SetupBonePtrs((MultiAnimFrame*)pFrame->pFrameFirstChild);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

HRESULT MultiAnimManager::Setup(LPDIRECT3DDEVICE9 pDevice,
	TCHAR sFile[],
	TCHAR sFxFile[],
	MultiAllocateHierarchy* pAH,
	LPD3DXLOADUSERDATA pLUD)
{
	pAH->SetMA(this);

	m_pd3dDevice = pDevice;
	m_pd3dDevice->AddRef();

	HRESULT hr;
	D3DXVECTOR3 vCenter;
	LPD3DXEFFECTCOMPILER pEC = NULL;

	D3DXMACRO mac[2] =
	{
		{"MATRIX_PALETTE_SIZE_DEFAULT", "35"},
		{NULL, NULL}
	};

	D3DCAPS9 caps;
	D3DXMACRO* pmac = NULL;
	m_pd3dDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	WCHAR wszPath[MAX_PATH];
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
		dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
		dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
	hr = D3DXCreateEffectFromFile(m_pd3dDevice,
			sFxFile,
			pmac,
			NULL,
			dwShaderFlags,
			NULL,
			&m_pEffect,
			NULL);

	if (FAILED(hr))
	{
		if (m_amxWorkingPalette)
		{
			delete[] m_amxWorkingPalette;
			m_amxWorkingPalette = NULL;
			m_dwWorkingPaletteSize = 0;
		}

		if (m_pAC)
		{
			m_pAC->Release();
			m_pAC = NULL;
		}

		if (m_pFrameRoot)
		{
			D3DXFrameDestroy(m_pFrameRoot, pAH);
			m_pFrameRoot = NULL;
		}

		if (m_pEffect)
		{
			m_pEffect->Release();
			m_pEffect = NULL;
		}

		if (pEC)
			pEC->Release();

		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
		return hr;
	}
	
	hr = D3DXLoadMeshHierarchyFromX(wszPath, 0, m_pd3dDevice, pAH, pLUD,
		(LPD3DXFRAME*)&m_pFrameRoot, &m_pAC);

	if (FAILED(hr))
	{
		if (m_amxWorkingPalette)
		{
			delete[] m_amxWorkingPalette;
			m_amxWorkingPalette = NULL;
			m_dwWorkingPaletteSize = 0;
		}

		if (m_pAC)
		{
			m_pAC->Release();
			m_pAC = NULL;
		}

		if (m_pFrameRoot)
		{
			D3DXFrameDestroy(m_pFrameRoot, pAH);
			m_pFrameRoot = NULL;
		}

		if (m_pEffect)
		{
			m_pEffect->Release();
			m_pEffect = NULL;
		}

		if (pEC)
			pEC->Release();

		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
		return hr;
	}

	if (!m_pAC)
	{
		hr = E_FAIL;
		MessageBox(NULL, L"Mesh load failed.", L"Error", MB_OK);
		if (FAILED(hr))
		{
			if (m_amxWorkingPalette)
			{
				delete[] m_amxWorkingPalette;
				m_amxWorkingPalette = NULL;
				m_dwWorkingPaletteSize = 0;
			}

			if (m_pAC)
			{
				m_pAC->Release();
				m_pAC = NULL;
			}

			if (m_pFrameRoot)
			{
				D3DXFrameDestroy(m_pFrameRoot, pAH);
				m_pFrameRoot = NULL;
			}

			if (m_pEffect)
			{
				m_pEffect->Release();
				m_pEffect = NULL;
			}

			if (pEC)
				pEC->Release();

			m_pd3dDevice->Release();
			m_pd3dDevice = NULL;
			return hr;
		}
	}

	hr = SetupBonePtrs(m_pFrameRoot);
	if (FAILED(hr))
	{
		if (m_amxWorkingPalette)
		{
			delete[] m_amxWorkingPalette;
			m_amxWorkingPalette = NULL;
			m_dwWorkingPaletteSize = 0;
		}

		if (m_pAC)
		{
			m_pAC->Release();
			m_pAC = NULL;
		}

		if (m_pFrameRoot)
		{
			D3DXFrameDestroy(m_pFrameRoot, pAH);
			m_pFrameRoot = NULL;
		}

		if (m_pEffect)
		{
			m_pEffect->Release();
			m_pEffect = NULL;
		}

		if (pEC)
			pEC->Release();

		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
		return hr;
	}

	hr = D3DXFrameCalculateBoundingSphere(m_pFrameRoot, &vCenter, &m_fBoundingRadius);
	if (FAILED(hr))
	{
		if (m_amxWorkingPalette)
		{
			delete[] m_amxWorkingPalette;
			m_amxWorkingPalette = NULL;
			m_dwWorkingPaletteSize = 0;
		}

		if (m_pAC)
		{
			m_pAC->Release();
			m_pAC = NULL;
		}

		if (m_pFrameRoot)
		{
			D3DXFrameDestroy(m_pFrameRoot, pAH);
			m_pFrameRoot = NULL;
		}

		if (m_pEffect)
		{
			m_pEffect->Release();
			m_pEffect = NULL;
		}

		if (pEC)
			pEC->Release();

		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
		return hr;
	}

	{
		std::vector<AnimInstance*>::iterator itCur, itEnd = m_v_pAnimInstances.end();
		for (itCur = m_v_pAnimInstances.begin(); itCur != itEnd; ++itCur)
		{
			LPD3DXANIMATIONCONTROLLER pNewAC = NULL;
			hr = m_pAC->CloneAnimationController(m_pAC->GetMaxNumAnimationOutputs(),
				m_pAC->GetMaxNumAnimationSets(),
				m_pAC->GetMaxNumTracks(),
				m_pAC->GetMaxNumEvents(),
				&pNewAC);
			// Release existing animation controller
			if ((*itCur)->m_pAC)
				(*itCur)->m_pAC->Release();
			(*itCur)->Setup(pNewAC);
		}
	}

}

HRESULT MultiAnimManager::Cleanup(MultiAllocateHierarchy* pAH)
{
	if (m_amxWorkingPalette)
	{
		delete[] m_amxWorkingPalette;
		m_amxWorkingPalette = NULL;
		m_dwWorkingPaletteSize = 0;
	}

	if (m_pAC)
	{
		m_pAC->Release();
		m_pAC = NULL;
	}

	if (m_pFrameRoot)
	{
		D3DXFrameDestroy(m_pFrameRoot, pAH);
		m_pFrameRoot = NULL;
	}

	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}

	if (m_pd3dDevice)
	{
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}

	return S_OK;
}

LPDIRECT3DDEVICE9 MultiAnimManager::GetDevice()
{
	m_pd3dDevice->AddRef();
	return m_pd3dDevice;
}

LPD3DXEFFECT MultiAnimManager::GetEffect()
{
	if (m_pEffect)
		m_pEffect->AddRef();

	return m_pEffect;
}

DWORD MultiAnimManager::GetNumInstances()
{
	return (DWORD)m_v_pAnimInstances.size();
}

AnimInstance* MultiAnimManager::GetInstance(DWORD dwIndex)
{
	return m_v_pAnimInstances[dwIndex];
}

float MultiAnimManager::GetBoundingRadius()
{
	return m_fBoundingRadius;
}

HRESULT MultiAnimManager::CreateNewInstance(DWORD* pdwNewIdx)
{
	AnimInstance* pAI;
	HRESULT hr = CreateInstance(&pAI);
	if (FAILED(hr))
		return hr;

	try
	{
		m_v_pAnimInstances.push_back(pAI);
	}
	catch (...)
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}

	*pdwNewIdx = (DWORD)m_v_pAnimInstances.size() - 1;
}

void MultiAnimManager::SetTechnique(char* sTechnique)
{
	m_sTechnique = sTechnique;
}

HRESULT MultiAnimManager::Draw()
{
	// TODO: modify this for much faster bulk rendering
	HRESULT hr = S_OK, hrT;

	std::vector<AnimInstance*>::iterator itCur, itEnd = m_v_pAnimInstances.end();
	for (itCur = m_v_pAnimInstances.begin(); itCur != itEnd; ++itCur);
	{
		if (FAILED(hrT = (*itCur)->Draw()))
			hr = hrT;
	}
	return hr;

}