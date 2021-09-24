#include "stdafx.h"
#include "Character.h"
#include "FBXLoad.h"

Character::Character()
{
}


Character::~Character()
{
}

void Character::OnInit(const char* filename, const char* TexFileName, LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light, float x, float y, float z, float size_x, float size_y, float size_z)
{
	fxOption = WithShadow;

	pLight = &light;

	m_pd3dDevice = pd3dDevice;

	std::pair<int, int> Counts; // LoadFBX�Լ��� ���� ���� ����, �ﰢ�� ����

	// ��ǥ ����
	m_x = x;
	m_y = y;
	m_z = z;
	m_sizeX = size_x;
	m_sizeY = size_y;
	m_sizeZ = size_z;

	m_Direction.x = 0.0f;
	m_Direction.y = 0.0f;
	m_Direction.z = -1.0f;

	Angle = 0.0f;
	// Fbx�ε�
	LoadFBX(filename, Counts, m_Vertices, m_Indices);

	// ���� ����, �ﰢ�� ���� ����
	VertexCount = m_Vertices.size();
	triCount = Counts.second;

	m_pd3dDevice->CreateVertexBuffer(VertexCount * sizeof(ModelVertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_MODELVERTEX, D3DPOOL_DEFAULT, &pVB, 0);
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
	//���̴� ����

	if (fxOption == OnlyMeterial)
	{
		m_ToonShader = LoadShader("ToonShaderA.fx", m_pd3dDevice);

		ZeroMemory(&mtrl, sizeof(mtrl));
		mtrl.Diffuse = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); //���ݻ� ��
		mtrl.Ambient = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); // ȯ�汤
		mtrl.Specular = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); // ���ݻ籤
		mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // ���� ���ϴ� ȿ��
		mtrl.Power = 2.0f;
	}
	if (fxOption == WithTexture)
	{
		m_ModelEffect = LoadShader("LightingTextureMapping.fx", m_pd3dDevice);
	}
	else if (fxOption == WithShadow)
	{
		const int shadowMapSize = 2048;

		m_ApplyShadowShader = LoadShader("ApplyShadowA.fx", m_pd3dDevice);
		m_CreateShadowShader = LoadShader("CreateShadowA.fx", m_pd3dDevice);

		if (FAILED(m_pd3dDevice->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_ShadowRenderTarget,
			NULL)))
		{
			MessageBox(NULL, L"Failed Make RenderTarget", L"Error", MB_OK);
			return;
		}

		if (FAILED(m_pd3dDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_ShadowDepthStencil, NULL)))
		{
			MessageBox(NULL, L"Failed Make DepthStencilSurface", L"Error", MB_OK);
			return;
		}
	}

	m_boundingBox.OnInit(m_pd3dDevice, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(-10.0f, -10.0f, -10.0f), D3DXVECTOR3(m_x, m_y, m_z), D3DXVECTOR3(m_sizeX, m_sizeY, m_sizeZ));
	
}

// �ؽ��� ���� �ܼ� ����
void Character::OnInit(const char* filename, LPDIRECT3DDEVICE9 pd3dDevice, Camera& camera, D3DLIGHT9& light, float x, float y, float z, float size_x, float size_y, float size_z)
{	
	HRESULT hr;

	fxOption = OnlyMeterial;

	pLight = &light;

	m_pd3dDevice = pd3dDevice;

	std::pair<int, int> Counts; // LoadFBX�Լ��� ���� ���� ����, �ﰢ�� ����

	// ��ǥ ����
	m_x = x;
	m_y = y;
	m_z = z;
	m_sizeX = size_x;
	m_sizeY = size_y;
	m_sizeZ = size_z;

	m_Direction.x = 0.0f;
	m_Direction.y = 0.0f;
	m_Direction.z = 1.0f;

	Angle = 0.0f;
	// Fbx�ε�
	LoadFBX(filename, Counts, m_Vertices, m_Indices);

	// ���� ����, �ﰢ�� ���� ����
	VertexCount = m_Vertices.size();
	triCount = Counts.second;

	hr = m_pd3dDevice->CreateVertexBuffer(VertexCount * sizeof(ModelVertex), D3DUSAGE_DYNAMIC, D3DFVF_MODELVERTEX, D3DPOOL_DEFAULT, &pVB, 0);
	hr = m_pd3dDevice->CreateIndexBuffer(sizeof(DWORD) * m_Indices.size(), D3DUSAGE_DYNAMIC, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pIB, 0);

	ModelVertex* paVertices;
	pVB->Lock(0, 0, (VOID**)&paVertices, D3DLOCK_DISCARD);
	memcpy(paVertices, &m_Vertices[0], sizeof(ModelVertex) * m_Vertices.size());
	pVB->Unlock();

	DWORD* paIndices;
	pIB->Lock(0, 0, (VOID**)&paIndices, D3DLOCK_DISCARD);
	memcpy(paIndices, &m_Indices[0], sizeof(DWORD) * m_Indices.size());
	pIB->Unlock();

	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); //���ݻ� ��
	mtrl.Ambient = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); // ȯ�汤
	mtrl.Specular = D3DXCOLOR(0.2f, 0.1f, 0.3f, 1.0f); // ���ݻ籤
	mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // ���� ���ϴ� ȿ��
	mtrl.Power = 2.0f;

	m_ToonShader = LoadShader("ToonShader.fx", m_pd3dDevice);

	m_boundingBox.OnInit(m_pd3dDevice, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(-10.0f, 0.0f, -10.0f), D3DXVECTOR3(m_x, m_y, m_z), D3DXVECTOR3(m_sizeX, m_sizeY, m_sizeZ));
}

void Character::OnRender(Camera& camera)
{

	D3DXMatrixScaling(&matScale, m_sizeX, m_sizeY, m_sizeZ);

	D3DXMatrixTranslation(&matTrans, m_x, m_y, m_z);
	//-90�� ȸ���ϴ� ������ ������ ������ ���� �� �� 90�� ȸ���Ͽ� ��Ÿ��.
	//���� ������ ���̰� ������ ���� ���Ͽ��� ���� -90�� ȸ�� �� ��.
	D3DXMatrixRotationX(&matRotationX, D3DXToRadian(-90));

	matWorld = matScale * matRotationX * matTrans;

	if (fxOption == OnlyMeterial)
	{
		D3DXMatrixRotationX(&matRotationX, D3DXToRadian(0));
		D3DXMatrixRotationY(&matRotationY, Angle);
		matWorld = matScale * matRotationX * matRotationY * matTrans;

		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//m_ToonShader->SetMatrix("gWorldMatrix", &matWorld);
		//m_ToonShader->SetMatrix("gViewMatrix", &camera.GetView());
		//m_ToonShader->SetMatrix("gProjectionMatrix", &camera.GetProj());

			// ��������� ������� ���Ѵ�.
		/*D3DXMATRIXA16 matInvWorld;
		D3DXMatrixTranspose(&matInvWorld, &matWorld);*/

			// ����/��/��������� �̸� ���Ѵ�.
		/*D3DXMATRIXA16 matWorldView;
		D3DXMATRIXA16 matWorldViewProjection;
		D3DXMatrixMultiply(&matWorldView, &matWorld, &camera.GetView());
		D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &camera.GetProj());

		m_ToonShader->SetMatrix("gWorldViewProjectionMatrix", &matWorldViewProjection);
		m_ToonShader->SetMatrix("gInvWorldMatrix", &matInvWorld);

		m_ToonShader->SetVector("gWorldLightPosition", &D3DXVECTOR4(pLight->Position.x, pLight->Position.y, pLight->Position.z, 0.0f));
*/
		m_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(ModelVertex));
		m_pd3dDevice->SetIndices(pIB);
		m_pd3dDevice->SetFVF(D3DFVF_MODELVERTEX);

		m_pd3dDevice->SetMaterial(&mtrl);

		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);

	/*	UINT numPasses = 0;
		m_ToonShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_ToonShader->BeginPass(i);
				{
				}
				m_ToonShader->EndPass();
			}
		}
		m_ToonShader->End();*/
		// ���� �ڽ� ����
		D3DXMatrixTranslation(&matTrans, m_x, m_y + 2.5f, m_z);
		D3DXMatrixIdentity(&matWorld);
		matWorld = matScale * matRotationX * matRotationY * matTrans;
		m_boundingBox.OnRender(matWorld);
	}

	if (fxOption == WithTexture)
	{
		m_ModelEffect->SetMatrix("gWorldMatrix", &matWorld);
		m_ModelEffect->SetMatrix("gViewMatrix", &camera.GetView());
		m_ModelEffect->SetMatrix("gProjectionMatrix", &camera.GetProj());

		m_ModelEffect->SetVector("gWorldLightPosition", &D3DXVECTOR4(pLight->Position.x, pLight->Position.y, pLight->Position.z, 0.0f));
		m_ModelEffect->SetVector("gWorldCameraPosition", &D3DXVECTOR4(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z, 0.0f));

		m_ModelEffect->SetTexture("DiffuseMap_Tex", m_ModelTexture);

		m_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(ModelVertex));
		m_pd3dDevice->SetIndices(pIB);
		m_pd3dDevice->SetFVF(D3DFVF_MODELVERTEX);

		UINT numPasses = 0;
		m_ModelEffect->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_ModelEffect->BeginPass(i);
				{
					m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);
				}
				m_ModelEffect->EndPass();
			}
		}
		m_ModelEffect->End();

	}
	//else if (fxOption == WithShadow) // �׸��� ����
	//{
	//	// ���� �����
	//	D3DXMATRIXA16 matLightView;
	//	{
	//		D3DXVECTOR3 vEyePt(pLight->Position.x, pLight->Position.y, pLight->Position.z);
	//		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &camera.GetAt(), &camera.GetUp());
	//	}

	//	// ���� �������
	//	D3DXMATRIXA16 matLightProjection;
	//	{
	//		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
	//	}

	//	D3DXMATRIXA16 matViewProjection;
	//	{
	//		// �����
	//		D3DXMATRIXA16 matView;
	//		D3DXVECTOR3 vEyePt(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z);
	//		D3DXMatrixLookAtLH(&matView, &vEyePt, &camera.GetAt(), &camera.GetUp());

	//		// �������
	//		D3DXMATRIXA16 matProjection;
	//		D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	//		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	//	}

	//	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	//	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	//	m_pd3dDevice->GetRenderTarget(0, &pHWBackBuffer);
	//	m_pd3dDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//	//////////////////////////
	//	// 1. �׸��� �����
	//	//////////////////////////

	//	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	//	if (SUCCEEDED(m_ShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	//	{
	//		m_pd3dDevice->SetRenderTarget(0, pShadowSurface);
	//		pShadowSurface->Release();
	//		pShadowSurface = NULL;
	//	}
	//	m_pd3dDevice->SetDepthStencilSurface(m_ShadowDepthStencil);

	//	m_pd3dDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	//	m_CreateShadowShader->SetMatrix("gWorldMatrix", &matWorld);
	//	m_CreateShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	//	m_CreateShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	//	m_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(ModelVertex));
	//	m_pd3dDevice->SetIndices(pIB);
	//	m_pd3dDevice->SetFVF(D3DFVF_MODELVERTEX);

	//	UINT numPasses = 0;
	//	m_CreateShadowShader->Begin(&numPasses, NULL);
	//	{
	//		for (UINT i = 0; i < numPasses; ++i)
	//		{
	//			m_CreateShadowShader->BeginPass(i);
	//			{
	//				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);
	//			}
	//			m_CreateShadowShader->EndPass();
	//		}
	//	}
	//	m_CreateShadowShader->End();

	//	//////////////////////////
	//	// 2. �׸��� ������
	//	//////////////////////////

	//	m_pd3dDevice->SetRenderTarget(0, pHWBackBuffer);
	//	m_pd3dDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	//	pHWBackBuffer->Release();
	//	pHWBackBuffer = NULL;
	//	pHWDepthStencilBuffer->Release();
	//	pHWDepthStencilBuffer = NULL;

	//	m_ApplyShadowShader->SetMatrix("gWorldMatrix", &matWorld);
	//	m_ApplyShadowShader->SetMatrix("gViewProjectionMatrix", &matViewProjection);

	//	m_ApplyShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	//	m_ApplyShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	//	m_ApplyShadowShader->SetVector("gWorldLightPosition", &D3DXVECTOR4(pLight->Position.x, pLight->Position.y, pLight->Position.z, 1.0f));
	//	
	//	m_ApplyShadowShader->SetTexture("ShadowMap_Tex", m_ShadowRenderTarget);
	//	m_ApplyShadowShader->SetTexture("Sampler2D_Tex", m_ModelTexture);

	//	numPasses = 0;
	//	m_ApplyShadowShader->Begin(&numPasses, NULL);
	//	{
	//		for (UINT i = 0; i < numPasses; ++i)
	//		{
	//			m_ApplyShadowShader->BeginPass(i);
	//			{
	//				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);
	//			}
	//			m_ApplyShadowShader->EndPass();
	//		}
	//	}
	//	m_ApplyShadowShader->End();
	//}
}

void Character::OnUpdate()
{
	if (CharacterMovingState == Character::MovingState::Nothing)
	{
		return;
	}
	if (CharacterMovingState == Character::MovingState::Rotating)
	{
		RotateY();
	}
}

void Character::OnUpdate(float x, float y, float z)
{
	if (CharacterMovingState == Character::MovingState::Nothing)
	{
		if (AbsoluteNum(m_x - x) > 0.5f || AbsoluteNum(m_y -y) > 0.5f || AbsoluteNum(m_z - z) > 0.5f)
		{
			CharacterMovingState = Character::MovingState::Moving;
		}
	}
	if (CharacterMovingState == Character::MovingState::Moving)
	{
		D3DXVECTOR3 pos_target(x, y, z);
		D3DXVECTOR3 pos_targetNormalize;
		D3DXVECTOR3 crossResult;
		D3DXVECTOR3 tmp;
		float angle;
		float speed = 5.0f;
		// ȸ��

		// ��ü�� �¿� ��ġ �Ǻ�
		// 1. Ÿ�ٰ� ������ ������ ���� -> ���� ���� ����
		// 2. ���� ���Ϳ� up(0, 1, 0)�� ������ ����
		// 3. ���� ��� ����� ���� ������ ������

		D3DXVec3Subtract(&tmp, &pos_target, &D3DXVECTOR3(m_x, m_y, m_z));

		if (m_PosTarget != pos_target)
		{
			m_PosTarget = pos_target;

			D3DXVec3Normalize(&pos_targetNormalize, &tmp);
			/*D3DXVec3Subtract(&pos_targetNormalize, &pos_target, &D3DXVECTOR3(m_x, m_y, m_z));*/
			D3DXVec3Cross(&crossResult, &pos_targetNormalize, &m_Direction);
			if (D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &crossResult) < 0)
			{
				Angle += acos(D3DXVec3Dot(&m_Direction, &pos_targetNormalize));
			}
			else
			{
				Angle += -acos(D3DXVec3Dot(&m_Direction, &pos_targetNormalize));
			}


			D3DXVec3Normalize(&m_Direction, &tmp);
		}
		

		// �̵�
		// TODO : m_PosTarget�� �μ����� ���̳ʽ��̰� ĳ������ ��ǥ�� Ŭ �� �̵����� ����.
		if (AbsoluteNum(m_x - m_PosTarget.x) < speed && AbsoluteNum(m_y - m_PosTarget.y) < speed && AbsoluteNum(m_z - m_PosTarget.z) < speed)
		{
			CharacterMovingState = Character::MovingState::Nothing;
			return;
		}
		m_x += m_Direction.x * speed;
		m_y += m_Direction.y * speed;
		m_z += m_Direction.z * speed;
	}
	if (CharacterMovingState == Character::MovingState::TestMoving)
	{
		float speedX = 0.05f;
		float speedY = 0.05f;
		float speedZ = 0.05f;

		// x ��ǥ�� �̵�
		// 1. ��ǥ Ȯ��
		if (m_x < x)
		{
			if (AbsoluteNum(m_x - x) < speedX)
			{
				speedX = AbsoluteNum(m_x - x);
			}
		}
		else if (m_x > x)
		{
			speedX = -speedX;
			if (AbsoluteNum(x - m_x) < speedX)
			{
				speedX = x - m_x;
			}
		}


		// y
		if (m_y < y)
		{
			if (AbsoluteNum(m_y - y) < speedY)
			{
				speedY = AbsoluteNum(m_y - y);
			}
		}
		else if (m_y > y)
		{
			speedY = -speedY;
			if (AbsoluteNum(y - m_y) < speedY)
			{
				speedY = y - m_y;
			}
		}

		// z
		if (m_z < z)
		{
			if (AbsoluteNum(m_z - z) < speedZ)
			{
				speedZ = AbsoluteNum(m_z - z);
			}
		}
		else if (m_z > z)
		{
			speedZ = -speedZ;
			if (AbsoluteNum(z - m_z) < speedZ)
			{
				speedZ = z - m_z;
			}
		}

		if (m_x != x)
		{
			MoveX(speedX);
		}

		if (m_y != y)
		{
			MoveY(speedY);
		}

		if (m_z != z)
		{
			MoveZ(speedZ);
		}
	}
}

void Character::OnRelease()
{
	m_boundingBox.OnRelease();

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

void Character::ShowInformation(LPD3DXFONT Font)
{
	RECT rt{ 10, 40, 0, 0 };

	char string[100];
	sprintf_s(string, "Charactor dir: x - %f , y - %f, z - %f", m_Direction.x, m_Direction.y, m_Direction.z);
	Font->DrawTextA(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	rt = { 10, 55, 0, 0 };

	sprintf_s(string, "Charactor PosTarget: x - %f , y - %f, z - %f", m_PosTarget.x, m_PosTarget.y, m_PosTarget.z);
	Font->DrawTextA(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	rt = { 10, 70, 0, 0 };

	sprintf_s(string, "Charactor Pos: x - %f , y - %f, z - %f", m_x, m_y, m_z);
	Font->DrawTextA(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
}

// ���� �� �Լ�
D3DXVECTOR3 Character::GetPos()
{
	return D3DXVECTOR3(m_x, m_y, m_z);
}

//---------------------------------------------------------------------------------------------------------------------------------------
// PRIVATE
// --------------------------------------------------------------------------------------------------------------------------------------

void Character::Rotate(int angle)
{

}

void Character::RotateX()
{

}

void Character::RotateY()
{
	Angle++;
}

void Character::MoveX(float speed)
{
	m_x += speed;
}

void Character::MoveY(float speed)
{
	m_y += speed;
}

void Character::MoveZ(float speed)
{
	m_z += speed;
}