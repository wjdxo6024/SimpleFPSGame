#include "stdafx.h"
#include "Building.h"


Building::Building()
{
}


Building::~Building()
{
}

void Building::OnInit(const char* filename, const char* TexFileName, LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light, float x, float y, float z, float size_x, float size_y, float size_z)
{
	pLight = &light;

	m_pd3dDevice = pd3dDevice;

	std::pair<int, int> Counts; // LoadFBX함수를 통해 얻을 정점, 삼각형 개수

	// 좌표 설정
	m_x = x;
	m_y = y;
	m_z = z;
	m_sizeX = size_x;
	m_sizeY = size_y;
	m_sizeZ = size_z;

	// Fbx로드
	LoadFBX(filename, Counts, m_Vertices, m_Indices);

	// 정점 개수, 삼각형 개수 설정
	VertexCount = m_Vertices.size();
	triCount = Counts.second;

	m_pd3dDevice->CreateVertexBuffer(VertexCount * sizeof(ModelVertex), D3DUSAGE_WRITEONLY, D3DFVF_MODELVERTEX, D3DPOOL_MANAGED, &pVB, 0);
	m_pd3dDevice->CreateIndexBuffer(sizeof(DWORD) * m_Indices.size(), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &pIB, 0);

	ModelVertex* paVertices;
	pVB->Lock(0, 0, (VOID**)&paVertices, 0);
	memcpy(paVertices, &m_Vertices[0], sizeof(ModelVertex) * m_Vertices.size());
	pVB->Unlock();

	DWORD* paIndices;
	pIB->Lock(0, 0, (VOID**)&paIndices, 0);
	memcpy(paIndices, &m_Indices[0], sizeof(DWORD) * m_Indices.size());
	pIB->Unlock();

	//Texture
	if (FAILED(D3DXCreateTextureFromFileA(m_pd3dDevice, TexFileName, &m_ModelTexture)))
	{
		MessageBox(NULL, L"Cannot load Image! Check Your Image", L"ImageLoadError", MB_OK);
	}

	//셰이더 생성

	m_ModelEffect = LoadShader("./Shader/LightingTextureMapping.fx", m_pd3dDevice);
	m_ModelEffect->SetVector("gLightColor", &D3DXVECTOR4(light.Diffuse.r, light.Diffuse.g, light.Diffuse.b, light.Diffuse.b));
	m_ModelEffect->SetVector("gWorldCameraPosition", &D3DXVECTOR4(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z, 1.0f));
	m_ModelEffect->SetVector("gWorldLightPosition", &D3DXVECTOR4(light.Position.x, light.Position.y, light.Position.z, 1.0f));

	//const int shadowMapSize = 2048;

	//m_ApplyShadowShader = LoadShader("ApplyShadowA.fx", m_pd3dDevice);
	//m_CreateShadowShader = LoadShader("CreateShadowA.fx", m_pd3dDevice);

	//if (FAILED(m_pd3dDevice->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_ShadowRenderTarget,
	//	NULL)))
	//{
	//	MessageBox(NULL, L"Failed Make RenderTarget", L"Error", MB_OK);
	//	return;
	//}

	//if (FAILED(m_pd3dDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_ShadowDepthStencil, NULL)))
	//{
	//	MessageBox(NULL, L"Failed Make DepthStencilSurface", L"Error", MB_OK);
	//	return;
	//}

}

void Building::OnRender(Camera& camera)
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, m_sizeX, m_sizeY, m_sizeZ);

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, m_x, m_y, m_z);
	//-90도 회전하는 이유는 블렌더로 파일을 추출 할 시 90도 회전하여 나타남.
	//만약 성능을 높이고 싶으면 블렌더 파일에서 직접 -90도 회전 할 것.
	D3DXMATRIXA16 matRotationX;
	D3DXMATRIXA16 matRotationY;
	D3DXMatrixRotationX(&matRotationX, D3DXToRadian(-90));

	matWorld = matScale * matRotationX * matTrans;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 텍스쳐
	m_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(ModelVertex));
	m_pd3dDevice->SetIndices(pIB);
	m_pd3dDevice->SetFVF(D3DFVF_MODELVERTEX);

	m_pd3dDevice->SetTexture(0, m_ModelTexture);

	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// 광원 뷰행렬
	//D3DXMATRIXA16 matLightView;
	//{
	//	D3DXVECTOR3 vEyePt(pLight->Position.x, pLight->Position.y, pLight->Position.z);
	//	D3DXMatrixLookAtLH(&matLightView, &vEyePt, &camera.GetAt(), &camera.GetUp());
	//}

	//// 광원 투영행렬
	//D3DXMATRIXA16 matLightProjection;
	//{
	//	D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
	//}

	//D3DXMATRIXA16 matViewProjection;
	//{
	//	// 뷰행렬
	//	D3DXMATRIXA16 matView;
	//	D3DXVECTOR3 vEyePt(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z);
	//	D3DXMatrixLookAtLH(&matView, &vEyePt, &camera.GetAt(), &camera.GetUp());

	//	// 투영행렬
	//	D3DXMATRIXA16 matProjection;
	//	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	//	D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	//}

	//LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	//LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	//m_pd3dDevice->GetRenderTarget(0, &pHWBackBuffer);
	//m_pd3dDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);


	//////////////////////////
	// 1. 그림자 만들기
	//////////////////////////

	/*LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(m_ShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		m_pd3dDevice->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	m_pd3dDevice->SetDepthStencilSurface(m_ShadowDepthStencil);

	m_pd3dDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	m_CreateShadowShader->SetMatrix("gWorldMatrix", &matWorld);
	m_CreateShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	m_CreateShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	m_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(ModelVertex));
	m_pd3dDevice->SetIndices(pIB);
	m_pd3dDevice->SetFVF(D3DFVF_MODELVERTEX);

	UINT numPasses = 0;*/
	/*m_CreateShadowShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_CreateShadowShader->BeginPass(i);
			{
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);
			}
			m_CreateShadowShader->EndPass();
		}
	}
	m_CreateShadowShader->End();*/

	//////////////////////////
	// 2. 그림자 입히기
	//////////////////////////

	/*m_pd3dDevice->SetRenderTarget(0, pHWBackBuffer);
	m_pd3dDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;

	m_ApplyShadowShader->SetMatrix("gWorldMatrix", &matWorld);
	m_ApplyShadowShader->SetMatrix("gViewProjectionMatrix", &matViewProjection);

	m_ApplyShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	m_ApplyShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	m_ApplyShadowShader->SetVector("gWorldLightPosition", &D3DXVECTOR4(pLight->Position.x, pLight->Position.y, pLight->Position.z, 1.0f));

	m_ApplyShadowShader->SetTexture("ShadowMap_Tex", m_ShadowRenderTarget);
	m_ApplyShadowShader->SetTexture("Sampler2D_Tex", m_ModelTexture);

	numPasses = 0;
	m_ApplyShadowShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_ApplyShadowShader->BeginPass(i);
			{
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);
			}
			m_ApplyShadowShader->EndPass();
		}
	}
	m_ApplyShadowShader->End();*/

}

void Building::OnUpdate()
{
}

void Building::OnRelease()
{
	if (m_ShadowRenderTarget != NULL)
	{
		m_ShadowRenderTarget->Release();
	}

	if (m_ShadowDepthStencil != NULL)
	{
		m_ShadowDepthStencil->Release();
	}

	if (m_ModelEffect != NULL)
	{
		m_ModelEffect->Release();
	}
	if (m_ModelTexture != NULL)
	{
		m_ModelTexture->Release();
	}

	if (Model != NULL)
	{
		Model->Release();
	}

	if (pVB != NULL)
	{
		pVB->Release();
	}

	if (pIB != NULL)
	{
		pIB->Release();
	}
}

std::pair<BOOL, D3DXVECTOR3> Building::IntersectRay(D3DXVECTOR3* Ray, D3DXVECTOR3* Origin)
{
	float u;
	float v;
	float dist;
	D3DXVECTOR4 tmpVec;
	D3DXVECTOR3 v0, v1, v2, PickPos;
	BOOL result;
	m_TestVertices.clear();

	for (int i = 0; i < VertexCount; i = i + 3)
	{
		D3DXVec3Transform(&tmpVec, &D3DXVECTOR3(m_Vertices[i]._x, m_Vertices[i]._y, m_Vertices[i]._z), &matWorld);
		v0 = D3DXVECTOR3(tmpVec.x, tmpVec.y, tmpVec.z);

		/*v0 = D3DXVECTOR3(m_Vertices[i]._x, m_Vertices[i]._y, m_Vertices[i]._z);*/

		D3DXVec3Transform(&tmpVec, &D3DXVECTOR3(m_Vertices[i + 1]._x, m_Vertices[i + 1]._y, m_Vertices[i + 1]._z), &matWorld);
		v1 = D3DXVECTOR3(tmpVec.x, tmpVec.y, tmpVec.z);

		/*v1 = D3DXVECTOR3(m_Vertices[i + 1]._x, m_Vertices[i + 1]._y, m_Vertices[i + 1]._z);*/

		D3DXVec3Transform(&tmpVec, &D3DXVECTOR3(m_Vertices[i + 2]._x, m_Vertices[i + 2]._y, m_Vertices[i + 2]._z), &matWorld);
		v2 = D3DXVECTOR3(tmpVec.x, tmpVec.y, tmpVec.z);

		//v2 = D3DXVECTOR3(m_Vertices[i + 2]._x, m_Vertices[i + 2]._y, m_Vertices[i + 2]._z);

		result = D3DXIntersectTri(&v0, &v1, &v2,
			Origin,
			Ray,
			&u,
			&v,
			&dist);
		if (result)
		{
			PickPos = v0 + u * (v1 - v0) + v * (v2 - v0);
			return std::pair<BOOL, D3DXVECTOR3>(TRUE, PickPos);
		}
	}

	return std::pair<BOOL, D3DXVECTOR3>(FALSE, PickPos);
}