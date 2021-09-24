#pragma once
#ifndef _MULTI_PLAYING_SCENE
#define _MULTI_PLAYING_SCENE
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>
#include <cstdio>
#include "Camera.h"
#include "CD3DApp.h"
#include "SkyBox.h"
#include "Building.h"
#include "CText.h"
#include "Player.h"
#include "OtherPlayer.h"
#include "Weapon.h"
#ifdef _DEBUG //fbxsdk 오류때문에 쓰고 있음.
#undef _DEBUG
#endif

class MultiPlaying
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;
	float timeDelta;

	// 빛
	D3DLIGHT9 light;

	//폰트
	LPD3DXFONT m_Font;
	CText m_Text;

	//FPS
	DWORD m_dwElapsedTime;
	double m_nFPS, m_nFPSCount;

	// 카메라
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up;

	Camera m_Camera;

	SkyBox m_SkyBox;

	//Model
	Building Ground;
	Building Wall;
	BoundingBox m_WallBoxs;

	//
	Player player;

	std::vector<OtherPlayer> OtherPlayer;
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	int OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnRelease();

	int HandleKeyBoard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif

