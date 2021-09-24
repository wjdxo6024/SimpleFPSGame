#pragma once
#include <d3d9.h>
#include <d3dx9core.h>
#include <Windows.h>
class CBox
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;
	LPD3DXLINE m_Line;
	D3DXVECTOR2 m_vList[5];
	D3DXCOLOR m_LineColor;
	int m_LineWidth;
	RECT m_rt;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, RECT rt, D3DXCOLOR color, int linewidth);
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height, D3DXCOLOR color, int linewidth);
	bool OnRender();
	bool OnUpdate();
	bool OnRelease();

	void SetLineWidth(int linewidth);
	void SetLineColor(D3DXCOLOR color);

	RECT GetRT();
};

