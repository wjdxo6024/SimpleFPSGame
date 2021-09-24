#pragma once
#ifndef _D3D9ENGINE_H_
#define _D3D9ENGINE_H_
#include "Mydef.h"
class D3D9Engine
{
private:
	HWND m_hWnd;
	LPDIRECT3D9 m_pD3D = NULL;
	LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
public:
	HRESULT InitD3D(HWND hWnd);
	void Render();
	void Cleanup();
	void Update();
	void BeginScene();
	void EndScene();
	LPDIRECT3DDEVICE9 GetDevice();
	D3D9Engine();
	~D3D9Engine();
};

#endif
