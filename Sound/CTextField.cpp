#include "CTextField.h"

bool CTextField::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height)
{
	m_IsFocus = false;
	m_Is_AlwaysShowBox = false;
	m_hWnd = hWnd;

	m_TextInform.OnInit(pd3dDevice, 20);
	m_Rect.left = x;
	m_Rect.top = y;
	m_Rect.right = x + width;
	m_Rect.bottom = y + height;

	ClearText();

	m_TextColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_TextInform.SetTextAlign(DT_WORDBREAK | DT_SINGLELINE);
	m_hIMC = ImmGetContext(hWnd);

	m_Box.OnInit(pd3dDevice, m_hWnd, m_Rect, m_TextColor, 2);

	return true;
}

bool CTextField::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height, int LimitString)
{
	m_IsFocus = false;
	m_Is_AlwaysShowBox = false;

	m_hWnd = hWnd;

	m_TextInform.OnInit(pd3dDevice, 20);
	m_Rect.left = x;
	m_Rect.top = y;
	m_Rect.right = x + width;
	m_Rect.bottom = y + height;

	ClearText();

	m_TextColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_hIMC = ImmGetContext(hWnd);

	m_Box.OnInit(pd3dDevice, m_hWnd, m_Rect, m_TextColor, 2);
	m_LimitString = LimitString;
	return true;
}

bool CTextField::OnRender()
{

	if (m_Is_AlwaysShowBox || m_IsFocus)
	{
		m_Box.OnRender();
	}
	m_TextInform.OnRender(m_Rect, m_wOutputString.c_str(), m_TextColor);
	return true;
}

// 키보드의 이벤트를 bool형태로 전송
bool CTextField::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos)
{
	switch (uMsg)
	{
		// 기본적인 입력
	case WM_CHAR:
	{
		switch (wParam)
		{
		case VK_RETURN:
			m_StringBuf = m_wOutputString;
			ClearText();
			return true;
		}
		break;
	}

	default:
		break;
	}

	return false;
}

bool CTextField::OnUpdateClient(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos)
{
	// HandleMouse(uMsg, wParam, lParam, MousePos);
	int len = 0;

	switch (uMsg)
	{
		// 기본적인 입력
	case WM_CHAR:
	{
		switch (wParam)
		{
		case VK_BACK:
			if (m_wOutputString.size() > 0)
				m_wOutputString.pop_back();
			break;
		default:
			// len = lstrlenW((TCHAR)wParam);
			if (m_LimitString == INFINITY_STRING || m_wOutputString.size() < m_LimitString)
				m_wOutputString += (WCHAR)wParam;
			break;
		}
		break;
	}

	default:
		break;
	}

	return false;
}

bool CTextField::OnRelease()
{
	ImmReleaseContext(m_hWnd, m_hIMC);

	m_TextInform.OnRelease();

	m_Box.OnRelease();

	return true;
}

bool CTextField::OnFocusIn(POINT MousePos)
{
	return PtInRect(&m_Rect, MousePos);
}

bool CTextField::OnFocusOut()
{
	return true;
}

bool CTextField::IsFocus()
{
	return m_IsFocus;
}

bool CTextField::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int len = 0;
	switch (uMsg)
	{
		// 이 코드는 한글을 기본 입력 마냥 실시간으로 렌더링할 때 사용. 현재 사용 X
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_COMPOSITION:
		{
			m_hIMC = ImmGetContext(m_hWnd);
			// 완성된 글자 출력
			if (lParam & GCS_RESULTSTR)
			{
				if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, m_InputWord, len)) > 0)
				{
					m_InputWord[len] = '\0';
					strcpy(m_resultWord + strlen(m_resultWord), m_InputWord);
					memset(m_InputWord, 0, 10);
					m_OutputString += m_resultWord;
				}
			}
			// 조합된 글자 출력
			if (lParam & GCS_COMPSTR)
			{
				len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
				ImmGetCompositionString(m_hIMC, GCS_COMPSTR, m_InputWord, len);
				m_InputWord[len] = '\0';
			}
		}
		case WM_IME_ENDCOMPOSITION:
		{
			break;
		}
		// 기본적인 입력
		case WM_CHAR:
		{
			switch (wParam)
			{
			case VK_RETURN: // 엔터를 누를 경우 참값을 반환시켜 버퍼에 데이터가 찼다는 걸 알림.
				m_StringBuf = m_wOutputString;
				ClearText();
				return true;
				break;
			case VK_BACK:
				if (m_wOutputString.size() > 0)
					m_wOutputString.pop_back();
				break;
			default:
				// len = lstrlenW((TCHAR)wParam);
				if(m_LimitString == INFINITY_STRING || m_wOutputString.size() < m_LimitString)
					m_wOutputString += (WCHAR)wParam;
				break;
			}
			break;
		}

		default:
			break;
	}
	return false;
}

bool CTextField::HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam,POINT MousePos)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		m_IsFocus = OnFocusIn(MousePos);
		break;
	case WM_MOUSEMOVE:

#ifdef _DEBUG
		char temp[10];
		m_MousePos.clear();
		m_MousePos += "X: ";
		m_MousePos += _itoa(MousePos.x, temp, 10);
		m_MousePos += "Y: ";
		m_MousePos += _itoa(MousePos.y, temp, 10);
		m_MousePos += " Focus: ";
		if (IsFocus())
			m_MousePos += "True";
		else
			m_MousePos += "False";
#endif
		break;
	}
	return true;
}

// private method

void CTextField::ClearText()
{
	m_OutputString.clear();
	m_wOutputString.clear();
	fflush(stdin);
}

void CTextField::ShowText()
{

}

void CTextField::ShowCursor()
{

}

bool CTextField::GetOutputString(std::wstring& dest)
{
	if (!m_StringBuf.empty())
	{
		dest = m_StringBuf;
		m_StringBuf.clear();
		return true;
	}
	else
	{
		m_StringBuf = m_wOutputString;
		dest = m_StringBuf;
		m_StringBuf.clear();
		return true;
	}
	return false;
}

bool CTextField::LimitInputChar(int limit)
{
	m_LimitString = limit;
	return true;
}

bool CTextField::IsBlank()
{
	return m_wOutputString.empty();
}

bool CTextField::AlwaysShowBox()
{
	if (m_Is_AlwaysShowBox)
		m_Is_AlwaysShowBox = false;
	else
		m_Is_AlwaysShowBox = true;
	return m_Is_AlwaysShowBox;
}