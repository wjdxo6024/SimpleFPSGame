#pragma once
#ifndef _GAMEEDU_H_
#define _GAMEEDU_H_

#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>
#include <cstdio>
#include "Camera.h"
#include "CD3DApp.h"
#include "SkyBox.h"
#include "Character.h"
#include "Building.h"
#include "Snow.h"
#include "Firework.h"
#include "ParticleGun.h"
#include "Hierarchy.h"
#include "Skinning.h"
#include "HeightMap.h"
#include "Terrain.h"
#include "Teapot.h"
#include "CText.h"
#include "Player.h"
#include "OtherPlayer.h"
// Skinned Mesh Test
#include "AllocateHierachy.h"
#include "AniMesh.h"

class TCPIPGame : public CD3DApp
{
private:

	virtual HRESULT OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	TCPIPGame();
	~TCPIPGame();

	void OnLeftMouseClick(); // 외부에서 불러옴.
private:

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

	Building WhiteTile;
	Building Map;
	

	//Particle
	Snow snow;
	Firework firework;
	ParticleGun Gun;

	//Picking
	D3DXVECTOR3 PickRayDir;
	D3DXVECTOR3 PickRayOrig;
	std::pair<BOOL, D3DXVECTOR3> result;

	// 메시지
	char RayMsg[100];
	char PickMsg[100];
	char OrigMsg[100];

	// Test
	Hierarchy TestObject;
	Skinning SkinningObject;
	HeightMap heightMap;
	Terrain m_Terrain;
	BOOL TestInit = FALSE;

	AniMesh TestAni;

	Player player;
	OtherPlayer OtherPlayer;
};

#endif
