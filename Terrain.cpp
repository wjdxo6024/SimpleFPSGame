#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
	m_cxHeight = 0;
	m_cyHeight = 0;
	m_czHeight = 0;
	m_vfScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pvHeightMap = NULL;
	m_pd3dDevice = NULL;
	/*for (int i = 0; i < MAX_TERRAIN_TEX; i++) m_pTex[i] = NULL;*/
	m_pTex = NULL;
	m_pVB = NULL;
	m_pIB = NULL;
	m_nTriangles = 0;
	m_pQuadTree = NULL;
}


Terrain::~Terrain()
{
}

HRESULT Terrain::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3* pvfScale, LPCSTR lpBMPFilename, LPCSTR lpTexFilename, Camera* pCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_Camera = pCamera;
	m_vfScale = *pvfScale;
	if (FAILED(BuildHeightMap(lpBMPFilename)))
	{
		OnRelease();
		return E_FAIL;
	}
	if (FAILED(LoadTextures(lpTexFilename)))
	{
		OnRelease();
		return E_FAIL;
	}
	if(FAILED(CreateVIB()))
	{
		OnRelease();
		return E_FAIL;
	}
	m_pQuadTree = new QuadTree(m_cxHeight, m_czHeight);
	if (FAILED(BuildQuadTree()))
	{
		OnRelease();
		return E_FAIL;
	}

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_pd3dDevice->SetMaterial(&mtrl);

	m_Frustum = new Frustum;
	m_fLODRatio = 0.1f;
	return S_OK;
}

HRESULT Terrain::OnRender()
{
	LPVOID pI;

	m_Frustum->Make((D3DXMATRIXA16*)&m_Camera->GetView());

	if (FAILED(m_pIB->Lock(0, (m_cxHeight - 1)*(m_czHeight - 1) * 2 * sizeof(TERRAININDEX), (void**)&pI, 0)))
		return E_FAIL;
	m_nTriangles = m_pQuadTree->GenerateIndex(pI, m_pvHeightMap, m_Frustum, m_fLODRatio);
	m_pIB->Unlock();
	Draw();

	return S_OK;
}

HRESULT Terrain::OnRelease()
{
	if (m_pQuadTree != NULL)
		m_pQuadTree->~QuadTree();
	if (m_pvHeightMap != NULL)
		delete m_pvHeightMap;
	if (m_pVB != NULL)
		m_pVB->Release();
	if (m_pIB != NULL)
		m_pIB->Release();
	if (m_pTex != NULL)
		m_pTex->Release();

	/*for (int i = 0; i < 4; i++) delete m_pTex[i];*/
	delete m_pQuadTree;
	delete m_Frustum;
	return S_OK;

}

HRESULT Terrain::LoadTextures(LPCSTR lpTexFilename)
{
	//for (int i = 0; i < 4; i++)
	//{
	//	D3DXCreateTextureFromFileA(m_pd3dDevice, lpTexFilename[i], &m_pTex[i]);
	//}

	D3DXCreateTextureFromFileA(m_pd3dDevice, lpTexFilename, &m_pTex);

	return S_OK;
}

HRESULT Terrain::BuildHeightMap(LPCSTR lpFilename)
{
	if (FAILED(D3DXCreateTextureFromFileExA(m_pd3dDevice, lpFilename,
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pTexHeight)))
		return E_FAIL;

	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT d3drc;
	m_pTexHeight->GetLevelDesc(0, &ddsd);
	m_cxHeight = ddsd.Height;
	m_czHeight = ddsd.Width;

	if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_cxHeight * m_cxHeight * sizeof(TERRAINVERTEX), 0, D3DFVF_TERRAINVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return E_FAIL;
	}

	m_pTexHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY); // 텍스처 메모리 락
	VOID* pVertices;

	if (FAILED(m_pVB->Lock(0, m_cxHeight*m_czHeight * sizeof(TERRAINVERTEX), (void**)&pVertices, 0)))
		return E_FAIL;

	m_pvHeightMap = new TERRAINVERTEX[m_cxHeight * m_czHeight];
	/*m_pFrustum = new Frustum;*/

	TERRAINVERTEX v;
	/*TERRAINVERTEX* pV = (TERRAINVERTEX*)pVertices;*/
	for (DWORD z = 0; z < m_czHeight; z++)
	{
		for (DWORD x = 0; x < m_cxHeight; x++)
		{
			v.p.x = ((float)x - m_cxHeight / 2.0f) * m_vfScale.x;

			v.p.z = (-((float)z - m_czHeight / 2.0f)) * m_vfScale.z;

			v.p.y = (((float)(*((LPDWORD)d3drc.pBits + x + z * (d3drc.Pitch / 4)) & 0x000000ff)) / 5.0f) * m_vfScale.y;

			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize(&v.n, &v.n);
			v.t.x = (float)x / (m_cxHeight - 1);
			v.t.y = (float)z / (m_czHeight - 1);
			/**pV++ = v;*/

			m_pvHeightMap[x + z * m_cxHeight] = v;
		}
	}

	m_pVB->Unlock();
	m_pTexHeight->UnlockRect(0);
}

HRESULT Terrain::BuildQuadTree()
{
	m_pQuadTree->Build(m_pvHeightMap);
	return S_OK;
}

HRESULT Terrain::Draw()
{
	/*m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);*/
	m_pd3dDevice->SetTexture(0, m_pTex);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); // 0번 텍스처 스테이지의 확대 필터
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	// 0 번 텍스쳐: 0번 텍스쳐 인덱스 사용

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(TERRAINVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_TERRAINVERTEX);
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_cxHeight * m_czHeight, 0, m_nTriangles);

	return S_OK;
}

HRESULT Terrain::CreateVIB()
{
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_cxHeight*m_czHeight * sizeof(TERRAINVERTEX), 0, D3DFVF_TERRAINVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		OnRelease();
		return E_FAIL;
	}

	VOID* pVertices;

	if (FAILED(m_pVB->Lock(0, m_cxHeight*m_czHeight * sizeof(TERRAINVERTEX), (void**)&pVertices, 0)))
	{
		OnRelease();
		return E_FAIL;
	}
	memcpy(pVertices, m_pvHeightMap, m_cxHeight * m_czHeight * sizeof(TERRAINVERTEX));
	m_pVB->Unlock();

	if (FAILED(m_pd3dDevice->CreateIndexBuffer((m_cxHeight - 1)*(m_czHeight - 1) * 2 * sizeof(TERRAININDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT,
		&m_pIB, NULL)))
	{
		OnRelease();
		return E_FAIL;
	}
}