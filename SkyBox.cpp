#include "stdafx.h"
#include "SkyBox.h"
#include "MyD3DUtil.h"

SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{

}

void SkyBox::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera)
{
	m_pd3dDevice = pd3dDevice;

	m_SkyBoxMesh = LoadModel("./Model/Box.x", m_pd3dDevice);

	m_SkyBoxEffect = LoadShader("./Shader/SkyBoxA.fx", m_pd3dDevice);
	if (!m_SkyBoxEffect)
	{
		return;
	}

	if (FAILED(D3DXCreateCubeTextureFromFileA(m_pd3dDevice, "./Texture/Snow_ENV.dds", &m_SkyTexture)))
	{
		return;
	}

}

void SkyBox::OnRender(Camera& camera)
{
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_SkyBoxEffect->SetVector("ViewPosition", &D3DXVECTOR4(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z, 1.0f));

	// 뷰/투영행렬을 만든다.
	D3DXMATRIXA16 matViewProjection;
	{
		// 뷰 행렬을 만든다.
		D3DXMATRIXA16 matView;
		D3DXVECTOR3 vEyePt(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z);
		D3DXVECTOR3 vLookatPt(camera.GetAt().x, camera.GetAt().y, camera.GetAt().z);
		D3DXVECTOR3 vUpVec(camera.GetUp().x, camera.GetUp().y, camera.GetUp().z);
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

		// 투영행렬을 만든다.
		D3DXMATRIXA16			matProjection;
		D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	}

	m_SkyBoxEffect->SetMatrix("ViewProjection", &matViewProjection);
	m_SkyBoxEffect->SetTexture("CubeTexture_Tex", m_SkyTexture);

	UINT numPasses = 0;
	m_SkyBoxEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_SkyBoxEffect->BeginPass(i);
			{
				m_SkyBoxMesh->DrawSubset(0);
			}
			m_SkyBoxEffect->EndPass();
		}
	}
	m_SkyBoxEffect->End();

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void SkyBox::OnUpdate()
{

}

void SkyBox::OnRelease()
{
	if (m_SkyTexture != NULL)
	{
		m_SkyTexture->Release();
	}

	if (m_SkyBoxEffect != NULL)
	{
		m_SkyBoxEffect->Release();
	}

	if (m_SkyBoxMesh != NULL)
	{
		m_SkyBoxMesh->Release();
	}
}