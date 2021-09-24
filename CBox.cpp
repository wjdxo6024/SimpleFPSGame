#include "CBox.h"

bool CBox::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, RECT rt, D3DXCOLOR color, int linewidth)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;
	SetLineColor(color);
	SetLineWidth(linewidth);

	m_rt = rt;

	m_vList[0] = D3DXVECTOR2(rt.left, rt.top);
	m_vList[1] = D3DXVECTOR2(rt.right, rt.top);
	m_vList[2] = D3DXVECTOR2(rt.right, rt.bottom);
	m_vList[3] = D3DXVECTOR2(rt.left, rt.bottom);
	m_vList[4] = D3DXVECTOR2(rt.left, rt.top);

	D3DXCreateLine(m_pd3dDevice, &m_Line);
	m_Line->SetWidth(m_LineWidth);

	return true;
}

bool CBox::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height, D3DXCOLOR color, int linewidth)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;
	SetLineColor(color);
	SetLineWidth(linewidth);

	m_rt.top = y;
	m_rt.left = x;
	m_rt.bottom = y + height;
	m_rt.right = x + width;

	m_vList[0] = D3DXVECTOR2(x, y);
	m_vList[1] = D3DXVECTOR2(x + width, y);
	m_vList[2] = D3DXVECTOR2(x + width, y + height);
	m_vList[3] = D3DXVECTOR2(x, y + height);
	m_vList[4] = D3DXVECTOR2(x, y);

	D3DXCreateLine(m_pd3dDevice, &m_Line);
	m_Line->SetWidth(m_LineWidth);

	return true;
}

bool CBox::OnRender()
{
	m_Line->Begin();
	m_Line->Draw(m_vList, 5, m_LineColor);
	m_Line->End();

	return true;
}

bool CBox::OnUpdate()
{
	return true;
}

bool CBox::OnRelease()
{
	if (m_Line != NULL)
		m_Line->Release();

	return true;
}

void CBox::SetLineWidth(int linewidth)
{
	m_LineWidth = linewidth;
}

void CBox::SetLineColor(D3DXCOLOR color)
{
	m_LineColor = color;
}

RECT CBox::GetRT()
{
	return m_rt;
}