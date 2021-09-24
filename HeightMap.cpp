#include "stdafx.h"
#include "HeightMap.h"


HeightMap::HeightMap()
{
}


HeightMap::~HeightMap()
{
}


void HeightMap::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, Camera* pCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_Camera = pCamera;

	if (FAILED(D3DXCreateTextureFromFileExA(m_pd3dDevice, "map129.bmp",
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pTexHeight)))
		return;

	if (FAILED(D3DXCreateTextureFromFileA(m_pd3dDevice, "Heightblend.png", &m_pTexDiffuse)))
		return;

	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT d3drc;
	m_pTexHeight->GetLevelDesc(0, &ddsd);
	m_cxHeight = 129;
	m_czHeight = 129;
	
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(ddsd.Width*ddsd.Height * sizeof(HMVERTEX), 0, D3DFVF_HMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return;
	}

	m_pTexHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY); // 텍스처 메모리 락
	VOID* pVertices;

	if (FAILED(m_pVB->Lock(0, m_cxHeight*m_czHeight * sizeof(HMVERTEX), (void**)&pVertices, 0)))
		return;

	m_pvHeightMap = new D3DXVECTOR3[m_cxHeight * m_czHeight];
	m_pFrustum = new Frustum;

	HMVERTEX v;
	HMVERTEX* pV = (HMVERTEX*)pVertices;
	for (DWORD z = 0; z < m_czHeight; z++)
	{
		for (DWORD x = 0; x < m_cxHeight; x++)
		{
			v.p.x = (float)x - m_cxHeight / 2.0f;

			v.p.z = -((float)z - m_czHeight / 2.0f);

			v.p.y = ((float)(*((LPDWORD)d3drc.pBits + x + z * (d3drc.Pitch / 4)) & 0x000000ff)) / 10.0f;

			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize(&v.n, &v.n);
			v.t.x = (float)x / (m_cxHeight - 1);
			v.t.y = (float)z / (m_czHeight - 1);
			*pV++ = v;

			m_pvHeightMap[z * m_cxHeight + x] = v.p;
		}
	}

	m_pVB->Unlock();
	m_pTexHeight->UnlockRect(0);


	// 인덱스
	if (FAILED(m_pd3dDevice->CreateIndexBuffer((m_cxHeight - 1) * (m_czHeight - 1) * 2 * sizeof(HMINDEX), 0, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &m_pIB, NULL)))
		return;

	HMINDEX i;
	HMINDEX* pI;
	if(FAILED(m_pIB->Lock(0, (m_cxHeight - 1)*(m_czHeight - 1) * 2 * sizeof(HMINDEX), (void**)&pI, 0)))
		return;

	for (DWORD z = 0; z < m_czHeight - 1; z++)
	{
		for (DWORD x = 0; x < m_cxHeight - 1; x++)
		{
			i._0 = (z*m_cxHeight + x);
			i._1 = (z*m_cxHeight + x + 1);
			i._2 = ((z + 1)*m_cxHeight + x);
			*pI++ = i;

			i._0 = ((z + 1)*m_cxHeight + x);
			i._1 = (z * m_cxHeight + x + 1);
			i._2 = ((z + 1)*m_cxHeight + x + 1);
			*pI++ = i;
		}
	}

	m_pIB->Unlock();

	m_nTriangles = 0;

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_pd3dDevice->SetMaterial(&mtrl);

}

void HeightMap::OnRender()
{
	Animate();

	m_pd3dDevice->SetTexture(0, m_pTexDiffuse);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	DrawMesh(&m_matAni);
	if (!m_bHideFrustum) m_pFrustum->Draw(m_pd3dDevice);
}

void HeightMap::OnUpdate()
{
	if (GetAsyncKeyState('1') < 0)
	{
		m_bHideFrustum = !m_bHideFrustum;
		m_bLockFrustum = !m_bLockFrustum;
	}
}

void HeightMap::OnRelease()
{
	if (m_pTexHeight != NULL)
		m_pTexHeight->Release();

	if (m_pTexDiffuse != NULL)
		m_pTexDiffuse->Release();

	if (m_pIB != NULL)
		m_pIB->Release();

	if (m_pVB != NULL)
		m_pVB->Release();

	delete m_pFrustum;
	delete[] m_pvHeightMap;
}

void HeightMap::Animate()
{
	D3DXMatrixIdentity(&m_matAni);

	D3DXMATRIXA16 m;
	D3DXMATRIXA16 pView;
	D3DXMATRIXA16 matProj;

	pView = m_Camera->GetView();
	
	m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m = pView * matProj;

	if (!m_bLockFrustum) m_pFrustum->Make(&m);
	
	
	ProcessFrustumCull();

	/*static DWORD t = 0;
	static bool flag = false;

	DWORD d = GetTickCount() % ((int)((D3DX_PI * 2) * 1000));

	D3DXMatrixRotationY(&m_matAni, d / 1000.0f);
	D3DXMatrixIdentity(&m_matAni);

	if (d < t)
		flag = !flag;
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, flag ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	t = d;*/
}

void HeightMap::DrawMesh(D3DXMATRIXA16* pMat)
{
	m_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(HMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_HMVERTEX);
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_cxHeight*m_czHeight, 0,
		m_nTriangles);
}

HRESULT HeightMap::ProcessFrustumCull()
{
	DWORD i[4];
	BOOL b[4];
	HMINDEX idx;
	HMINDEX* pI;

	if (FAILED(m_pIB->Lock(0, (m_cxHeight - 1)*(m_czHeight - 1) * 2 * sizeof(HMINDEX), (void**)&pI, 0)))
		return E_FAIL;

	m_nTriangles = 0;

	for (DWORD z = 0; z < m_czHeight - 1; z++)
	{
		for (DWORD x = 0; x < m_cxHeight - 1; x++)
		{
			i[0] = (z * m_cxHeight + x);
			i[1] = (z * m_cxHeight + x + 1);
			i[2] = ((z + 1)*m_cxHeight + x);
			i[3] = ((z + 1)*m_cxHeight + x + 1);

			b[0] = m_pFrustum->IsIn(&m_pvHeightMap[i[0]]); // 좌측 상단 정점이 절두체 안에 있는가?
			b[1] = m_pFrustum->IsIn(&m_pvHeightMap[i[1]]); // 우측 상단 정점이 절두체 안에 있는가?
			b[2] = m_pFrustum->IsIn(&m_pvHeightMap[i[2]]); // 좌측 하단 정점이 절두체 안에 있는가?

			if (b[0] | b[1] | b[2])
			{
				idx._0 = i[0];
				idx._1 = i[1];
				idx._2 = i[2];
				*pI++ = idx;
				m_nTriangles++;
			}

			b[2] = m_pFrustum->IsIn(&m_pvHeightMap[i[2]]);
			b[1] = m_pFrustum->IsIn(&m_pvHeightMap[i[1]]);
			b[3] = m_pFrustum->IsIn(&m_pvHeightMap[i[3]]);

			if (b[2] | b[1] | b[3])
			{
				idx._0 = i[2];
				idx._1 = i[1];
				idx._2 = i[3];

				*pI++ = idx;
				m_nTriangles++;
			}
		}
	}

	m_pIB->Unlock();
	return S_OK;
}

void HeightMap::FrustumOn()
{
	if(GetAsyncKeyState('1') < 0)
	{
		m_bHideFrustum = !m_bHideFrustum;
		m_bLockFrustum = !m_bLockFrustum;
	}
}