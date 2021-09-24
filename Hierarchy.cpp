#include "stdafx.h"
#include "Hierarchy.h"


Hierarchy::Hierarchy()
{
}


Hierarchy::~Hierarchy()
{
}

void Hierarchy::OnInit(LPDIRECT3DDEVICE9  pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	CUSTOMVERTEX vertices[] =
	{
		{-1, 1, 1, 0xffff0000},
		{1, 1, 1, 0xff00ff00},
		{1, 1, -1, 0xff0000ff},
		{-1, 1, -1, 0xffffff00},

		{-1, -1, 1, 0xff00ffff},
		{1, -1, 1, 0xffff00ff},
		{1, -1, -1, 0xff000000},
		{-1, -1, -1, 0xffffffff}
	};

	if (FAILED(m_pd3dDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return;
	}

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
	{
		return;
	}
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();

	MYINDEX indices[] =
	{
		{0, 1, 2}, {0, 2, 3},
		{4, 6, 5}, {4, 7, 6},
		{0, 3, 7}, {0, 7, 4},
		{1, 5, 6}, {1, 6, 2},
		{3, 2, 6}, {3, 6, 7},
		{0, 4, 5}, {0, 5, 1}
	};

	if (FAILED(m_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return;
	}

	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		return;
	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	InitAnimate();

	return;
}
void Hierarchy::OnRender()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIXA16 matWorld;

	Animate();
	matWorld = m_matRParent * m_matTMParent;
	DrawMesh(&matWorld);

	matWorld = m_matRChild * m_matTMChild * m_matRParent * m_matTMParent;

	matWorld = m_matRChild * m_matTMChild * matWorld;
	DrawMesh(&matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Hierarchy::OnUpdate()
{

}

void Hierarchy::OnRelease()
{
	if (m_pIB != NULL)
		m_pIB->Release();

	if (m_pVB != NULL)
		m_pVB->Release();
}

void Hierarchy::InitAnimate()
{
	m_aniPos[0] = D3DXVECTOR3(0, 0, 0);
	m_aniPos[1] = D3DXVECTOR3(5, 5, 5);

	FLOAT Yaw = D3DX_PI * 90.0f / 180.0f;
	FLOAT Pitch = 0;
	FLOAT Roll = 0;
	D3DXQuaternionRotationYawPitchRoll(&m_aniRot[0], Yaw, Pitch, Roll);

	Yaw = 0;
	Pitch = D3DX_PI * 90.0f / 180.0f;
	Roll = 0;
	D3DXQuaternionRotationYawPitchRoll(&m_aniRot[1], Yaw, Pitch, Roll);
}

void Hierarchy::Animate()
{
	static float t = 0;
	float x, y, z;
	D3DXQUATERNION quat;

	if (t > 1.0f) t = 0.0f;

	x = Linear(m_aniPos[0].x, m_aniPos[1].x, t);
	y = Linear(m_aniPos[0].y, m_aniPos[1].y, t);
	z = Linear(m_aniPos[0].z, m_aniPos[1].z, t);

	D3DXMatrixTranslation(&m_matTMParent, x, y, z);

	D3DXQuaternionSlerp(&quat, &m_aniRot[0], &m_aniRot[1], t);
	D3DXMatrixRotationQuaternion(&m_matRParent, &quat);

	t += 0.005f;

	D3DXMatrixRotationZ(&m_matRChild, GetTickCount() / 500.0f);

	D3DXMatrixTranslation(&m_matTMChild, 3, 3, 3);
}

void Hierarchy::DrawMesh(D3DXMATRIXA16* pMat)
{
	m_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pd3dDevice->SetIndices(m_pIB);

	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}