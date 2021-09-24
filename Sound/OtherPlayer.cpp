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
	// 카메라 설정

	// 모델 로드
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

	// 쉐이더 설정
	// m_ModelEffect = LoadShader("TextureMapping.fx", m_pd3dDevice);

	// 텍스쳐 로드
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
	mtrl.Diffuse = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); //난반사 광
	mtrl.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); // 환경광
	mtrl.Specular = D3DXCOLOR(0.0f, 0.5f, 0.9f, 1.0f); // 정반사광
	mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // 빛을 발하는 효과
	mtrl.Power = 2.0f;

	//Emissive : 빛을 발하는 효과를 나타냄, 눈에 띄는 효과로는 빛을 받지않는 곳의 표면이 설정된 값으로 나타남.
	//ex) 표면이 기본으로 초록색이고 Emissive가 파란색일 때 그림자로 지는 부분이 파란색으로 나타남.
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
	//-90도 회전하는 이유는 블렌더로 파일을 추출 할 시 90도 회전하여 나타남.
	//만약 성능을 높이고 싶으면 블렌더 파일에서 직접 -90도 회전 할 것.
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

	 //투명 텍스쳐 처리 부분
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