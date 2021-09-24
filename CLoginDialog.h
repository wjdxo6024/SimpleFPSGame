#pragma once
#include "CDialog.h"
#include "CTextField.h"
#include "CTextButton.h"
#include "CBox.h"

class CLoginDialog : public CDialog
{
private:
	int m_x, m_y, m_width, m_height;
	CBox m_dialogDomain;
	CTextField m_NickNameField;
	CTextButton m_SummitButton;
	CTextButton m_ExitButton;
#ifdef  _DEBUG
	CTextButton m_FreeParticipate;
#endif //  _DEBUG
	CText m_Texts;
	std::wstring m_NickName;
	bool m_Focus;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int x, int y, int width, int height);
	bool OnRender();
	ButtonEventDefine OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT MousePos);
	bool OnRelease();

	std::wstring SummitNickName();
};

