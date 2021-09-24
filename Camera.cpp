#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{

}


Camera::~Camera()
{
}

void Camera::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND& hWnd)
{
	m_pd3dDevice = pd3dDevice;
	
	// 마우스 초기화
	POINT pt;
	GetCursorPos(&pt);

	GetClientRect(m_hWnd, &rc);
	pt.x = (rc.right - rc.top) / 2;

	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;

	//카메라 초기 설정
	m_hWnd = hWnd;
	D3DVIEWPORT9 vp;
	GetClientRect(m_hWnd, &rc);
	// 마우스 커서 숨기기
	ShowCursor(false);
	D3DXMATRIX matWorld;

	vp.X = 0;
	vp.Y = 0;
	vp.Width = rc.right - rc.left;
	vp.Height = rc.bottom - rc.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_Eye.x = 0.0f;
	m_Eye.y = 500.0f; // 100, 5.0f
	m_Eye.z = -500.0f; // -200 , -20.0f

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	SetMatrix(&m_Eye, &m_At, &m_Up);

	D3DXMatrixPerspectiveFovLH(&m_matProj, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pd3dDevice->SetViewport(&vp);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 200.0f);
}

void Camera::OnUpdate()
{
	POINT pt;
	float fDelta = 0.001f;

	// FPS
	GetCursorPos(&pt);
	int dx = pt.x - m_dwMouseX;
	int dy = pt.y - m_dwMouseY;

	this->RotationLocalX(dy * fDelta);
	this->RotationLocalY(dx * fDelta);

	// z = 0.4f로 움직였고 x는 0.25로 움직였음.
	if (GetAsyncKeyState('W') & 0x8000f)
		this->MoveLocalZ(2.0f);
	if (GetAsyncKeyState('S'))
		this->MoveLocalZ(-2.0f);
	if (GetAsyncKeyState('A'))
		this->MoveLocalX(-2.0f);
	if (GetAsyncKeyState('D'))
		this->MoveLocalX(2.0f);
	//if (GetAsyncKeyState('Q'))
	//	this->fly(0.5f);
	//if (GetAsyncKeyState('E'))
	//	this->fly(-0.5f);
	/*if (GetAsyncKeyState(VK_RIGHT))
		this->RotationLocalY(0.05f);
	if (GetAsyncKeyState(VK_LEFT))
		this->RotationLocalY(-0.05f);
	if (GetAsyncKeyState(VK_UP))
		this->RotationLocalX(-0.025f);
	if (GetAsyncKeyState(VK_DOWN))
		this->RotationLocalX(0.025f)*/;

	D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);

	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	SetCursor(NULL);
	GetClientRect(m_hWnd, &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(m_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;
}

void Camera::OnRelease()
{

}

D3DXMATRIX Camera::GetView()
{
	return m_matView;
}

void Camera::SetView(float x, float y, float z)
{

}

D3DXMATRIX Camera::GetProj()
{
	return m_matProj;
}

void Camera::SetProj(float x, float y, float z)
{

}

D3DXVECTOR3 Camera::GetEye()
{
	return m_Eye;
}

void Camera::SetEye(float x, float y, float z)
{
	m_Eye.x = x;
	m_Eye.y = y;
	m_Eye.z = z;
}

D3DXVECTOR3 Camera::GetAt()
{
	return m_At;
}

D3DXVECTOR3 Camera::GetUp()
{
	return m_Up;
}

D3DXVECTOR3 Camera::GetCross()
{
	return m_vCross;
}

D3DXVECTOR3 Camera::GetvView()
{
	return m_vView;
}

// 카메라 이동

void Camera::SetMatrix(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp)
{
	m_Eye = *pvEye;
	m_At = *pvLookat;
	m_Up = *pvUp;
	D3DXVec3Normalize(&m_vView, &(m_At - m_Eye));
	D3DXVec3Cross(&m_vCross, &m_Up, &m_vView);
}

void Camera::MoveLocalZ(float dist)
{
	D3DXVECTOR3 vNewEye = m_Eye;
	D3DXVECTOR3 vNewDst = m_At;
	D3DXVECTOR3 vNewView;
	D3DXVECTOR3 vMove;

	D3DXVec3Normalize(&vMove, D3DXVec3Normalize(&vNewView, &(m_At - m_Eye))); // vView = m_At - m_Eye
	vMove *= dist;
	//vNewEye += vMove;
	//vNewDst += vMove;
	vNewEye.x += vMove.x;
	vNewEye.z += vMove.z;

	vNewDst.x += vMove.x;
	vNewDst.z += vMove.z;
	return SetMatrix(&vNewEye, &vNewDst, &m_Up);
}

void Camera::MoveLocalX(float dist)
{
	D3DXVECTOR3 vNewEye = m_Eye;
	D3DXVECTOR3 vNewDst = m_At;
	D3DXVECTOR3 vNewView;
	D3DXVECTOR3 vMove;

	D3DXVec3Normalize(&vMove, D3DXVec3Normalize(&vNewView, &m_vCross));
	vMove *= dist;
	//vNewEye += vMove;
	//vNewDst += vMove;
	vNewEye.x += vMove.x;
	vNewEye.z += vMove.z;

	vNewDst.x += vMove.x;
	vNewDst.z += vMove.z;

	return SetMatrix(&vNewEye, &vNewDst, &m_Up);
}

void Camera::RotationLocalY(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_Up, angle);

	D3DXVECTOR3 vNewDst;
	D3DXVECTOR3 vNewView;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);
	vNewDst += m_Eye;

	return SetMatrix(&m_Eye, &vNewDst, &m_Up);
}

void Camera::RotationLocalX(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vCross, angle);

	D3DXVECTOR3 vNewDst;
	D3DXVECTOR3 vNewView;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);
	vNewDst += m_Eye;

	return SetMatrix(&m_Eye, &vNewDst, &m_Up);
}

//void Camera::MoveLocalY(float dist)
//{
//	D3DXVECTOR3 vNewEye = m_Eye;
//	D3DXVECTOR3 vNewDst = m_At;
//
//	D3DXVECTOR3 vMove;
//
//	D3DXVec3Normalize(&vMove, &m_Up);
//	vMove *= dist;
//	vNewEye += vMove;
//	vNewDst += vMove;
//
//	D3DXMatrixLookAtLH(&m_matView, &vNewEye, &vNewDst, &m_Up);
//}
//

