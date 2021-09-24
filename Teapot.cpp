#include "stdafx.h"
#include "Teapot.h"
#include "MyD3DUtil.h"

Teapot::Teapot()
{
}


Teapot::~Teapot()
{
}
void Teapot::OnInit(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); //난반사 광
	mtrl.Ambient = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); // 환경광
	mtrl.Specular = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); // 정반사광
	mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // 빛을 발하는 효과
	mtrl.Power = 2.0f;

	m_Texture = LoadTexture("./SkinnedMesh/Tiny_skin.dds", m_pd3dDevice);
	m_Teapot = LoadModel("./SkinnedMesh/tiny.x", m_pd3dDevice);
}

void Teapot::OnRender()
{
	D3DXMATRIX matWorld, matRotationY , matRotationX ,matTrans, matScale;
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);// 와이어 프레임 모드
	D3DXMatrixRotationX(&matRotationX, D3DXToRadian(-90));
	D3DXMatrixTranslation(&matTrans, 0.0f, 3.0f, 0.0f);
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	matWorld = matScale * matRotationX * matTrans;

	m_pd3dDevice->SetTexture(0, m_Texture);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pd3dDevice->SetMaterial(&mtrl);
	m_Teapot->DrawSubset(0);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void Teapot::OnUpdate()
{

}

void Teapot::OnRelease()
{
	if (m_Teapot != NULL)
	{
		m_Teapot->Release();
	}
}

void Teapot::Animate()
{

}

void Teapot::DrawMesh()
{

}
