#pragma once
#include <d3d9.h>
#include <Windows.h>
#include <d3dx9math.h>
#include "MyDef.h"

class Camera
{
public:

	enum CameraType { FPS, AIRCRAFT };

	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND& hWnd);
	void OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, int playerX, int playerY, int playerZ, CameraType cameraType);
	void OnUpdate();
	void OnRelease();

	D3DXMATRIX GetView();
	void SetView(float x, float y, float z);
	D3DXMATRIX GetProj();
	void SetProj(float x, float y, float z);

	D3DXVECTOR3 GetEye();
	void SetEye(float x, float y, float z);

	D3DXVECTOR3 GetAt();
	D3DXVECTOR3 GetUp();

	D3DXVECTOR3 GetCross();
	D3DXVECTOR3 GetvView();

	void SetMatrix(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp);
	//¿Ãµø
	void MoveLocalZ(float dist);
	void MoveLocalY(float dist);
	void MoveLocalX(float dist);

	void RotationLocalY(float angle);
	void RotationLocalX(float angle);

	Camera();
	~Camera();

private:
	CameraType cameraType;

	LPDIRECT3DDEVICE9 m_pd3dDevice;

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	D3DXVECTOR3 m_vCross;
	D3DXVECTOR3 m_vView;


	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up;

	DWORD m_dwMouseX;
	DWORD m_dwMouseY;

	RECT rc;
	HWND m_hWnd;
};

