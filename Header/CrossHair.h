#pragma once
#include <d3d9.h>
#include <d3dx9core.h>
class CrossHair
{
private:
	LPD3DXLINE m_Line;
	D3DXVECTOR2 m_vList[8];
	D3DXVECTOR2 m_HitLineList[8];
	D3DXCOLOR m_LineColor;

	int m_LineWidth;
	float m_CenterX = 0.0f;
	float m_CenterY = 0.0f;

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;

	RECT m_CrossHairOuterRect;
	RECT m_CrossHairInnerRect;

public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);

	bool OnRender();
	bool OnRender(bool IsHit);

	bool OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnRelease();
};

