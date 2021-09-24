#include "CSprite.h"

void CSprite::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* path)
{
	/*namespace fs = std::experimental::filesystem::v1;
	fs::create_directories(path);*/
	m_pd3dDevice = pd3dDevice;
	
	if (FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice, (LPCWSTR)path,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0), NULL, NULL,
		&m_pTexture)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Load Texture Sprite.", L"Error", MB_OK);
#endif
	}
	if (FAILED(D3DXCreateSprite(m_pd3dDevice, &m_Sprite)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Sprite.", L"Error", MB_OK);
#endif
}
}

// TODO : 현재 변환을 제대로 하지 않아 1이외에 size 변환시 출력 불가.
void CSprite::OnRender(int x, int y, int size)
{
	
	D3DXVECTOR3 position(x, y, 0);

	D3DXMATRIX mat;
	D3DXMatrixScaling(&mat, (float)size, (float)size, 0.0f);

	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_Sprite->SetTransform(&mat);

	m_Sprite->Draw(m_pTexture, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));

	m_Sprite->End();
}

void CSprite::OnUpdate()
{

}

void CSprite::OnRelease()
{
	if (m_Sprite != NULL)
	{
		m_Sprite->Release();
	}

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
	}
}