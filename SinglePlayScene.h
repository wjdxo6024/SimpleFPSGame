#pragma once
#ifndef _SINGLE_PLAY_SCENE
#define _SINGLE_PLAY_SCENE
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
#include "FPSInterface.h"
#include "CInGameChat.h"
#include "CKillDeathEventList.h"

#ifdef _DEBUG //fbxsdk ���������� ���� ����.
#undef _DEBUG
#endif

class SinglePlayScene
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND m_hWnd;
	float timeDelta;

	// ��
	D3DLIGHT9 light;

	//��Ʈ
	LPD3DXFONT m_Font;
	CText m_Text;

	//FPS
	DWORD m_dwElapsedTime;
	double m_nFPS, m_nFPSCount;

	// ī�޶�
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

	/*
	0~3 ���� �� ��
	4~5 
	*/
	BoundingBox m_WallBoxs[24];

	Player player;
	OtherPlayer OtherPlayer;

	//Weapons
	Weapon m_PlayerWeapons; // ����
	Sound m_ShootSound;
	int IndexWeapon;

	// �������̽�
	FPSInterface m_PlayerInterface;
	CKillDeathEventList m_KillDeathEventList;

	// ä��
	CInGameChat m_GameChat;

private:
	bool SetWallBoundingBox();
public:
	bool OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	bool OnRender();
	int OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnRelease();

	int HandleKeyBoard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif