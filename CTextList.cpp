#include "CTextList.h"

bool CTextList::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height)
{
	m_hWnd = hWnd;

	m_Is_AlwaysShowBox = false;
	m_IsFocus = false;

	m_TextInform.OnInit(pd3dDevice, 20);
	m_Rect.left = x;
	m_Rect.top = y;
	m_Rect.right = x + width;
	m_Rect.bottom = y + height;

	ClearList();

	m_TextColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_Box.OnInit(pd3dDevice, m_hWnd, m_Rect, m_TextColor, 2);

	ListSize = height / m_TextInform.GetFontSize();
	m_ListCurShow = 0;

	return true;
}

// 일정한 간격에 맞춰 출력 ex) 문자열이 일정이상 길이를 넘어갈 때 나눠서 출력
bool CTextList::OnRender()
{
	int AllTextHeight = 0;

	std::vector<std::wstring>::iterator it = m_StringList.begin() + m_ListCurShow;
	for (int i = 0; i < ListSize; i++)
	{
		if (it != m_StringList.end())
		{
			m_TextInform.OnRender(m_Rect.left, (m_Rect.top + i * m_TextInform.GetFontSize()), it->c_str(), m_TextColor);
		}
		else
			break;
		it++;
	}

	if (m_Is_AlwaysShowBox || m_IsFocus)
	{
		m_Box.OnRender();
	}
	return true;
}

bool CTextList::OnUpdate()
{
	if (m_StringList.size() > ListSize)
	{
		m_ListCurShow = m_StringList.size() - ListSize;
	}
	return true;

}

bool CTextList::OnRelease()
{
#ifdef _DEBUG
	m_Box.OnRelease();
#endif
	m_TextInform.OnRelease();
	return true;
}

std::vector<std::wstring>& CTextList::GetList()
{
	return m_StringList;
}

// index 값이 리스트의 크기보다 클 경우 list의 end()를 반환
std::wstring& CTextList::GetListByIndex (int index)
{
	if (m_StringList.size() < index)
		return *m_StringList.end();
	else
		return m_StringList[index];
}

// Rect의 크기와 폰트의 크기에 따라 조정하여 출력
void CTextList::PushElement(std::wstring dest)
{
	int RectWidth = m_Rect.right - m_Rect.left;

	if (CustomStrlen(dest, m_TextInform.GetFontSize()) > RectWidth)
	{
		int TempSize = dest.size();
		int cur = 0;
		int StringSize = 0;
		while (TempSize > 0)
		{
			std::wstring StringTmp;
			StringSize = 0;

			for (std::wstring::iterator it = (dest.begin() + cur); StringSize < RectWidth && it != dest.end(); cur++, it++)
			{
				StringTmp.push_back(*it);
				StringSize += GetCharacterSize(*it, m_TextInform.GetFontSize());
			}

			m_StringList.push_back(StringTmp);
			TempSize -= StringSize;
		}
		// 나머지 저장
		std::wstring StringTmp;
		StringTmp = dest.substr(cur, dest.size() - cur);
		m_StringList.push_back(StringTmp);
	}
	else
	{
		m_StringList.push_back(dest);
	}
}

bool CTextList::DeleteElement(int index)
{
	if (m_StringList.size() < index)
		return false;
	m_StringList.erase(m_StringList.begin() + index);
}

void CTextList::ClearList()
{
	m_StringList.clear();
}

void CTextList::SetTextColor(D3DXCOLOR TextColor)
{
	m_TextColor = TextColor;
}

bool CTextList::SetFocus()
{
	if (m_IsFocus)
		m_IsFocus = false;
	else
		m_IsFocus = true;
	return m_IsFocus;
}
bool CTextList::AlwaysShowBox()
{
	if (m_Is_AlwaysShowBox)
		m_Is_AlwaysShowBox = false;
	else
		m_Is_AlwaysShowBox = true;
	return m_Is_AlwaysShowBox;
}