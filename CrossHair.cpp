#include "CrossHair.h"

bool CrossHair::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;
	D3DVIEWPORT9 ClientViewPort;
	pd3dDevice->GetViewport(&ClientViewPort);
	m_CenterX = (float)ClientViewPort.Width / 2;
	m_CenterY = (float)ClientViewPort.Height / 2;
	

	// 정사각형이므로 Height와 Width가 서로 같아야한다.
	int HairLength = (int)ClientViewPort.Width / 32;
	int ShootBoxLength = (int)ClientViewPort.Width / 128;
	m_CrossHairOuterRect.left = (int)ClientViewPort.Width / 2 - HairLength;
	m_CrossHairOuterRect.right = (int)ClientViewPort.Width / 2 + HairLength;
	m_CrossHairOuterRect.top = (int)ClientViewPort.Height / 2 - HairLength;
	m_CrossHairOuterRect.bottom = (int)ClientViewPort.Height / 2 + HairLength;

	m_CrossHairInnerRect.left = (int)ClientViewPort.Width / 2 - ShootBoxLength;
	m_CrossHairInnerRect.right = (int)ClientViewPort.Width / 2 + ShootBoxLength;
	m_CrossHairInnerRect.top = (int)ClientViewPort.Height / 2 - ShootBoxLength;
	m_CrossHairInnerRect.bottom = (int)ClientViewPort.Height / 2 + ShootBoxLength;

	m_LineColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); // Green

	// CrossHair Line
	// ↓
	m_vList[0] = D3DXVECTOR2((float)ClientViewPort.Width / 2, (float)m_CrossHairOuterRect.top);
	m_vList[1] = D3DXVECTOR2((float)ClientViewPort.Width / 2, (float)m_CrossHairInnerRect.top);

	// ↑
	m_vList[2] = D3DXVECTOR2((float)ClientViewPort.Width / 2, (float)m_CrossHairInnerRect.bottom);
	m_vList[3] = D3DXVECTOR2((float)ClientViewPort.Width / 2, (float)m_CrossHairOuterRect.bottom);

	// →
	m_vList[4] = D3DXVECTOR2(m_CrossHairOuterRect.left, (float)ClientViewPort.Height / 2);
	m_vList[5] = D3DXVECTOR2(m_CrossHairInnerRect.left, (float)ClientViewPort.Height / 2);

	// ←
	m_vList[6] = D3DXVECTOR2(m_CrossHairOuterRect.right, (float)ClientViewPort.Height / 2);
	m_vList[7] = D3DXVECTOR2(m_CrossHairInnerRect.right, (float)ClientViewPort.Height / 2);

	// Hit CrossHair Line : TODO


	// Create Line
	m_LineWidth = 1;
	D3DXCreateLine(m_pd3dDevice, &m_Line);
	m_Line->SetWidth(m_LineWidth);
	return true;
}

bool CrossHair::OnRender()
{
	// CrossHair
	m_Line->Begin();
	m_Line->Draw(m_vList, 2, m_LineColor);
	m_Line->Draw(m_vList + 2, 2, m_LineColor);
	m_Line->Draw(m_vList + 4, 2, m_LineColor);
	m_Line->Draw(m_vList + 6, 2, m_LineColor);
	m_Line->End();

	return true;
}

bool CrossHair::OnRender(bool IsHit)
{
	m_Line->Begin();
	if (IsHit) // Hit CrossHair
	{

	}
	m_Line->Draw(m_vList, 8, m_LineColor);
	m_Line->End();
	return true;
}

bool CrossHair::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return true;
}

bool CrossHair::OnRelease()
{
	if (m_Line != NULL)
		m_Line->Release();
	return true;
}