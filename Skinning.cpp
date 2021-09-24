#include "stdafx.h"
#include "Skinning.h"


Skinning::Skinning()
{
}


Skinning::~Skinning()
{
}


void Skinning::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	if (FAILED(m_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(SKINNINGVERTEX), 0, D3DFVF_SKINNINGVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return;
	}

	SKINNINGVERTEX* pVertices;
	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return;

	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI*i / (50 - 1));


		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		pVertices[2 * i + 0].b[0] = 1.0f;
		pVertices[2 * i + 0].b[1] = 0.0f;
		pVertices[2 * i + 0].b[2] = 0.0f;
		pVertices[2 * i + 0].index = 0x0000;

		pVertices[2 * i + 0].color = 0xffffffff;
		pVertices[2 * i + 0].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 0].tv = 1.0f;

		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
		pVertices[2 * i + 1].b[0] = 0.5f;
		pVertices[2 * i + 1].b[1] = 0.5f;
		pVertices[2 * i + 1].b[2] = 0.0f;
		pVertices[2 * i + 1].index = 0x0001;

		pVertices[2 * i + 1].color = 0xff808080;
		pVertices[2 * i + 1].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 1].tv = 0.0f;
	}

	m_pVB->Unlock();

	D3DXCreateTextureFromFileA(m_pd3dDevice, "Water.jpg", &m_pTexture);
}

void Skinning::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

	D3DXMATRIXA16 matWorld;

	Animate();

	m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(0), &m_mat0);
	m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(1), &m_mat1);
	m_pd3dDevice->SetTexture(0, m_pTexture);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	DrawMesh();

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	/*m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);*/
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
}

void Skinning::OnUpdate()
{

}

void Skinning::OnRelease()
{
	if (m_pTexture != NULL)
		m_pTexture->Release();

	if (m_pIB != NULL)
		m_pIB->Release();

	if (m_pVB != NULL)
		m_pVB->Release();
}

void Skinning::Animate()
{
	D3DXMatrixIdentity(&m_mat0);

	DWORD d = GetTickCount() % ((int)(D3DX_PI * 2) * 1000);

	D3DXMatrixRotationY(&m_mat1, d / 500.0f);
}

void Skinning::DrawMesh()
{
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SKINNINGVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_SKINNINGVERTEX);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 1);
}