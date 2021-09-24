#include "stdafx.h"
#include "Sphere.h"


Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}

void Sphere::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, float Rad, float X, float Y, float Z)
{
	m_curX = X;
	m_curY = Y;
	m_curZ = Z;
	m_Rad = Rad;

	//ZeroMemory(&mtrl, sizeof(mtrl));

	//mtrl.Diffuse = D3DXCOLOR(0.2f, 1.0f, 0.4f, 1.0f); //³­¹Ý»ç ±¤
	//mtrl.Ambient = D3DXCOLOR(0.2f, 1.0f, 0.4f, 1.0f); // È¯°æ±¤
	//mtrl.Specular = D3DXCOLOR(0.2f, 1.0f, 0.4f, 1.0f); // Á¤¹Ý»ç±¤
	//mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // ºûÀ» ¹ßÇÏ´Â È¿°ú
	//mtrl.Power = 2.0f;

	m_Degree = 0;
	m_pd3dDevice = pd3dDevice;

	D3DXCreateSphere(m_pd3dDevice, m_Rad, 20, 20, &m_pSphereMesh, NULL);
}

void Sphere::OnRender()
{
	//D3DXMATRIX  matWorld, matTrans, matRotationY, matScaling;
	//D3DXMatrixIdentity(&matWorld);

	//m_pd3dDevice->SetMaterial(&mtrl);
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//D3DXMatrixTranslation(&matTrans, m_curX, m_curY, m_curZ);
	//D3DXMatrixRotationY(&matRotationY, GetTickCount() * 0.002f);
	//matWorld = matRotationY * matTrans * matRotationY;
	//m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pSphereMesh->DrawSubset(0);

}

void Sphere::OnUpdate()
{

}

void Sphere::OnRelease()
{
	if(m_pTexture != NULL)
		m_pTexture->Release();

	if (m_pSphereMesh != NULL)
		m_pSphereMesh->Release();

	if (m_pVB != NULL)
		m_pVB->Release();
}

float Sphere::GetRad()
{
	return m_Rad;
}

D3DXVECTOR3 Sphere::GetCenter()
{
	return D3DXVECTOR3(m_curX, m_curY, m_curZ);
}