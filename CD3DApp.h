#ifndef _CD3DAPP_H_
#define _CD3DAPP_H_
#include <d3d9.h>
#include <d3dx9math.h>
class CD3DApp
{
public:
	HRESULT InitD3D(HWND hWnd);
	void Render();
	void Cleanup();
	void Update();
	void BeginScene();
	void EndScene();
	LPDIRECT3DDEVICE9 GetDevice();
	CD3DApp();
	~CD3DApp();

protected:
	virtual HRESULT OnInit() = 0;
	virtual void OnRender() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRelease() = 0;

protected:
	HWND m_hWnd;
	LPDIRECT3D9 m_pD3D = NULL;
	LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
};

#endif
