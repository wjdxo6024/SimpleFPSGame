#include "stdafx.h"
#include "OtherPlayer.h"
OtherPlayer::OtherPlayer()
{
}


OtherPlayer::~OtherPlayer()
{
}

void OtherPlayer::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* texture)
{
	m_curX = 0.0f;
	m_curY = 100.0f;
	m_curZ = 0.0f;

	m_sizeX = 100.0f;
	m_sizeY = 100.0f;
	m_sizeZ = 100.0f;

	nLife = 100;

	dwBulletFireTime = 400;
	dwOldBulletFireTime = GetTickCount();

	m_pd3dDevice = pd3dDevice;
	// ī�޶� ����

	// �� �ε�
	std::pair<int, int> Counts;

	LoadFBX("./Model/Plane.fbx", Counts, m_Vertices, m_Indices);

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
	// m_ModelEffect = LoadShader("TextureMapping.fx", m_pd3dDevice);

	// �ؽ��� �ε�
	//Texture
	if (FAILED(D3DXCreateTextureFromFileA(m_pd3dDevice, "./Sprite/GreenCharacter/03.png", &m_pSprite[0])))
	{
		MessageBox(NULL, L"Cannot load Image! Check Your Image", L"ImageLoadError", MB_OK);
	}

//	if (FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice, L"TestPlayerF.dds",
//		D3DX_DEFAULT,
//		D3DX_DEFAULT,
//		D3DX_DEFAULT,
//		D3DUSAGE_RENDERTARGET,
//		D3DFMT_UNKNOWN,
//		D3DPOOL_DEFAULT,
//		D3DX_DEFAULT,
//		D3DX_DEFAULT,
//		D3DCOLOR_XRGB(0, 0, 0), NULL, NULL,
//		&m_pTexture[0])))
//	{
//#ifdef _DEBUG
//		MessageBox(NULL, L"Cannot Load Texture Sprite.", L"Error", MB_OK);
//#endif
//	}

	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); //���ݻ� ��
	mtrl.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); // ȯ�汤
	mtrl.Specular = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); // ���ݻ籤
	mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // ���� ���ϴ� ȿ��
	mtrl.Power = 2.0f;

	//Emissive : ���� ���ϴ� ȿ���� ��Ÿ��, ���� ��� ȿ���δ� ���� �����ʴ� ���� ǥ���� ������ ������ ��Ÿ��.
	//ex) ǥ���� �⺻���� �ʷϻ��̰� Emissive�� �Ķ����� �� �׸��ڷ� ���� �κ��� �Ķ������� ��Ÿ��.
}

void OtherPlayer::OnRender(Camera* camera, D3DLIGHT9* light)
{
	//-----------------------------------------------------------
	D3DXMATRIX matBillBoard;
	D3DXMatrixIdentity(&matBillBoard);
	matBillBoard._11 = camera->GetView()._11;
	matBillBoard._13 = camera->GetView()._13;
	matBillBoard._31 = camera->GetView()._31;
	matBillBoard._33 = camera->GetView()._33;
	D3DXMatrixInverse(&matBillBoard, NULL, &matBillBoard);

	D3DXMATRIX matRotationX, matRotationY, matWorld, matTrans, matScaling;
	D3DXMatrixScaling(&matScaling, m_sizeX, m_sizeY, m_sizeZ);

	D3DXMatrixTranslation(&matTrans, m_curX, m_curY, m_curZ);
	//-90�� ȸ���ϴ� ������ ������ ������ ���� �� �� 90�� ȸ���Ͽ� ��Ÿ��.
	//���� ������ ���̰� ������ ���� ���Ͽ��� ���� -90�� ȸ�� �� ��.
	D3DXMatrixRotationX(&matRotationX, D3DXToRadian(180));

	// D3DXMatrixRotationY(&matRotationY, D3DXToRadian());
	//D3DXMatrixIdentity(&matRotationX);
	D3DXMatrixIdentity(&matRotationY);
	matWorld = matScaling * matRotationX * matRotationY * matTrans * matBillBoard;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(ModelVertex));
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->SetFVF(D3DFVF_MODELVERTEX);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetMaterial(&mtrl);

	m_pd3dDevice->SetTexture(0, m_pSprite[0]);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	 //���� �ؽ��� ó�� �κ�
	//m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 0x0);
	//m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCount, 0, triCount);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void OtherPlayer::OnUpdate()
{

}

void OtherPlayer::OnUpdate(D3DXVECTOR3& pos, D3DXVECTOR3& dir)
{
	
}

void OtherPlayer::OnRelease()
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