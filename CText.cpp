#include "CText.h"

void CText::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, int fontsize = 20)
{
	m_pd3dDevice = pd3dDevice;
	m_fontsize = fontsize;
	D3DXCreateFont(m_pd3dDevice, fontsize, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"System", &m_Font);
}

void CText::OnRender(int x, int y, const char* string, D3DXCOLOR fontColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))
{
	RECT rc = { x, y, strlen(string) * 5, m_fontsize};
	m_Font->DrawTextA(NULL, string, -1, &rc, m_TextAlign, fontColor);
}

void CText::OnRender(RECT& rc, const char* string, D3DXCOLOR fontColor)
{
	m_Font->DrawTextA(NULL, string, -1, &rc, m_TextAlign, fontColor);
}

void CText::OnRender(int x, int y, int width, int height, const char* string, D3DXCOLOR fontColor)
{
	RECT rc = { x, y, x + width, y + height };
	m_Font->DrawTextA(NULL, string, -1, &rc, m_TextAlign, fontColor);
}

void CText::OnRender(int x, int y, const wchar_t* string, D3DXCOLOR fontColor)
{
	RECT rc = { x, y, lstrlen(string) * 5, m_fontsize };
	m_Font->DrawTextW(NULL, string, -1, &rc, m_TextAlign, fontColor);
}

void CText::OnRender(RECT& rc, const wchar_t* string, D3DXCOLOR fontColor)
{
	m_Font->DrawTextW(NULL, string, -1, &rc, m_TextAlign, fontColor);
}

void CText::OnRender(int x, int y, int width, int height, const wchar_t* string, D3DXCOLOR fontColor)
{
	RECT rc = { x, y, x + width, y + height };
	m_Font->DrawTextW(NULL, string, -1, &rc, m_TextAlign, fontColor);
}

void CText::OnUpdate()
{

}

void CText::OnRelease()
{
	if (m_Font != NULL)
	{
		m_Font->Release();
	}
}

void CText::SetColor()
{

}

void CText::SetFont(const char* font)
{

}

int CText::GetFontSize()
{
	return m_fontsize;
}

void CText::SetTextAlign(DWORD TextAlign)
{
	m_TextAlign = TextAlign;
}

void CText::SetFontSize(int fontsize)
{
	m_fontsize = fontsize;
}