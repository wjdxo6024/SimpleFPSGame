#include "stdafx.h"
#include "Player.h"
Player::Player()
{
}


Player::~Player()
{
}

void Player::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* texture, Camera* camera, HWND hWnd)
{
	m_curX = 0.0f;
	m_curY = 300.0f;
	m_curZ = -5000.0f;

	m_sizeX = 100.0f;
	m_sizeY = 100.0f;
	m_sizeZ = 100.0f;

	nLife = 100;

	dwBulletFireTime = 400;
	dwOldBulletFireTime = GetTickCount();

	m_pd3dDevice = pd3dDevice;
	// ī�޶� ����
	if (camera == NULL)
	{
		m_playerCamera = new Camera;
		m_playerCamera->OnInit(m_pd3dDevice, hWnd);
		m_playerCamera->SetMatrix(&D3DXVECTOR3(m_curX, m_curY, m_curZ), 
			&D3DXVECTOR3(20.0f, m_curY, 5000.0f), &(D3DXVECTOR3(0.0f, 1.0f, 0.0f)));
	}
	else
	{
		m_playerCamera = camera;
	}
	
	// �� �ε�
	std::pair<int, int> Counts;

	LoadFBX("./Model/Plane.fbx" , Counts, m_Vertices, m_Indices);

	VertexCount = m_Vertices.size();
	triCount = Counts.second;

	m_pd3dDevice->CreateVertexBuffer(VertexCount * sizeof(ModelVertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_MODELVERTEX, D3DPOOL_DEFAULT, &m_pVB, 0);
	m_pd3dDevice->CreateIndexBuffer(sizeof(DWORD) * m_Indices.size(), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, 0);

	ModelVertex* paVertices;
	m_pVB->Lock(0, 0, (VOID**)&paVertices, D3DLOCK_DISCARD);
	memcpy(paVertices, &m_Vertices[0], sizeof(ModelVertex) * m_Vertices.size());
	m_pVB->Unlock();

	DWORD* paIndices;
	m_pIB->Lock(0, 0, (VOID**)&paIndices, D3DLOCK_DISCARD);
	memcpy(paIndices, &m_Indices[0], sizeof(DWORD) * m_Indices.size());
	m_pIB->Unlock();

	// ���̴� ����
	m_ModelEffect = LoadShader("./Shader/TextureMapping.fx", m_pd3dDevice);

	// �ؽ��� �ε�
	//Texture
	/*if (FAILED(D3DXCreateTextureFromFileA(m_pd3dDevice, "TestPlayerF.png", &m_TestTexture)))
	{
		MessageBox(NULL, L"Cannot load Image! Check Your Image", L"ImageLoadError", MB_OK);
	}*/

	if (FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice, L"./Sprite/01.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0), NULL, NULL,
		&m_pTexture[0])))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Load Texture Sprite.", L"Error", MB_OK);
#endif
	}

	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); //���ݻ� ��
	mtrl.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); // ȯ�汤
	mtrl.Specular = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); // ���ݻ籤
	mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // ���� ���ϴ� ȿ��
	mtrl.Power = 2.0f;

	//Emissive : ���� ���ϴ� ȿ���� ��Ÿ��, ���� ��� ȿ���δ� ���� �����ʴ� ���� ǥ���� ������ ������ ��Ÿ��.
	//ex) ǥ���� �⺻���� �ʷϻ��̰� Emissive�� �Ķ����� �� �׸��ڷ� ���� �κ��� �Ķ������� ��Ÿ��.

}

void Player::OnRender()
{
	//-----------------------------------------------------------
	D3DXMATRIX matRotationX, matRotationY, matWorld, matTrans, matScaling;
	D3DXMatrixScaling(&matScaling, m_sizeX, m_sizeY, m_sizeZ);

	D3DXMatrixTranslation(&matTrans, m_curX, m_curY, m_curZ);
	//-90�� ȸ���ϴ� ������ ������ ������ ���� �� �� 90�� ȸ���Ͽ� ��Ÿ��.
	//���� ������ ���̰� ������ ���� ���Ͽ��� ���� -90�� ȸ�� �� ��.
	// D3DXMatrixRotationX(&matRotationX, D3DXToRadian(-90));

	// D3DXMatrixRotationY(&matRotationY, D3DXToRadian());
	D3DXMatrixIdentity(&matRotationX);
	D3DXMatrixIdentity(&matRotationY);
	matWorld = matScaling * matRotationX * matRotationY * matTrans;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// �ؽ��� ����
	m_ModelEffect->SetMatrix("gWorldMatrix", &matWorld);
	m_ModelEffect->SetMatrix("gViewMatrix", &m_playerCamera->GetView());
	m_ModelEffect->SetMatrix("gProjectionMatrix", &m_playerCamera->GetProj());

	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(ModelVertex));
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->SetFVF(D3DFVF_MODELVERTEX);

	m_ModelEffect->SetTexture("DiffuseMap_Tex", m_TestTexture);

	UINT numPasses = 0;
	m_ModelEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_ModelEffect->BeginPass(i);
			{
				// m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);
			}
			m_ModelEffect->EndPass();
		}
	}
	m_ModelEffect->End();
}

void Player::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_playerCamera->OnUpdate();
	m_curX = m_playerCamera->GetEye().x;
	m_curY = m_playerCamera->GetEye().y;
	m_curZ = m_playerCamera->GetEye().z;
}

void Player::OnRelease()
{
	if (m_playerCamera != NULL)
	{
		m_playerCamera->OnRelease();
		delete m_playerCamera;
	}

	if (m_ModelEffect != NULL)
	{
		m_ModelEffect->Release();
	}

	if (m_pTexture != NULL)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_pTexture[i] != NULL)
			{
				m_pTexture[i]->Release();
			}
		}
	}

	if (m_pIB != NULL)
	{
		m_pIB->Release();
	}

	if (m_pVB != NULL)
	{
		m_pVB->Release();
	}
}

D3DXVECTOR3 Player::GetVec()
{
	return D3DXVECTOR3(m_curX, m_curY, m_curZ);
}

float Player::GetRad()
{
	return m_ColJud;
}

int Player::GetLife()
{
	return nLife;
}

void Player::Damage()
{
	nLife--;
}

DWORD Player::GetFireTime()
{
	return dwBulletFireTime;
}
DWORD Player::GetOldFireTime()
{
	return dwOldBulletFireTime;
}

void Player::SetOldFireTime(DWORD nTime)
{
	dwOldBulletFireTime = nTime;
}

void Player::MoveX()
{
	
}

void Player::MoveY()
{

}

void Player::MoveZ()
{

}

Camera* Player::GetCamera()
{
	return  m_playerCamera;
}

D3DXVECTOR3 Player::GetPlayerPos()
{
	return D3DXVECTOR3(m_curX, m_curY, m_curZ);
}