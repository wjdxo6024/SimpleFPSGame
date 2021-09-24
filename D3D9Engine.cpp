#include "D3D9Engine.h"


void D3D9Engine::Render()
{
	if (NULL == m_pd3dDevice)
		return;

	m_pd3dDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0x00000000, 1.0f, 0L);// Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0)
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		// OnRender();
		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present(0, 0, 0, 0);
}

void D3D9Engine::BeginScene()
{
	if (NULL == m_pd3dDevice)
		return;
	m_pd3dDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0L);// Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0)
	m_pd3dDevice->BeginScene();
}

void D3D9Engine::EndScene()
{
	m_pd3dDevice->EndScene();
	m_pd3dDevice->Present(0, 0, 0, 0);
}

void D3D9Engine::Update()
{
	
}

void D3D9Engine::Cleanup()
{
	if (m_pd3dDevice != NULL)
		m_pd3dDevice->Release();
	if (m_pD3D != NULL)
		m_pD3D->Release();
}

HRESULT D3D9Engine::InitD3D(HWND hWnd)
{
	m_hWnd = hWnd;
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// 안티에일리어싱 msaa 2x
	if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, D3DFMT_R8G8B8, FALSE,
		D3DMULTISAMPLE_2_SAMPLES, NULL)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; //D3DMULTISAMPLE_NONE
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24X8
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	// ---------------------- z 버퍼 설정 부분 ------------------------ //
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice)))
	{
		return E_FAIL;
	}

	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// OnInit();

	return S_OK;
}

LPDIRECT3DDEVICE9 D3D9Engine::GetDevice()
{
	return m_pd3dDevice;
}

D3D9Engine::D3D9Engine()
{

}

D3D9Engine::~D3D9Engine()
{

}