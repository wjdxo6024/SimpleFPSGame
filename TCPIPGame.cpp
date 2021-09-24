#include "stdafx.h"
#include "TCPIPGame.h"
#include "MyDef.h"
#include <iostream>


TCPIPGame::TCPIPGame()
{
}


TCPIPGame::~TCPIPGame()
{
}

HRESULT TCPIPGame::OnInit()
{
	timeDelta = 0.01f;

	m_Camera.OnInit(m_pd3dDevice, m_hWnd);

	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;			/// 광원의 종류(점 광원,방향성 광원,스포트라이트)
	light.Diffuse.r = 1.0f;							/// 광원의 색깔과 밝기
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Position = D3DXVECTOR3(0.0f, 1000.0f, 35.0f);
	light.Direction = (D3DXVECTOR3(100.0f, 0.0f, 100.0f));
	light.Range = 1000.0f;

	m_pd3dDevice->SetLight(0, &light);
	m_pd3dDevice->LightEnable(0, true);

	//m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	m_SkyBox.OnInit(m_pd3dDevice, m_Camera);
	
	// 모델들.


	// 맵

	//Map.OnInit("Map_v1_0.fbx", "Plants.jpg", m_pd3dDevice, m_Camera, light, 0.0f, 0.0f, 0.0f, 50.0f, 50.0f, 50.0f);

	WhiteTile.OnInit("Plane.fbx", "WhiteTile.jpg", m_pd3dDevice, m_Camera, light, 0.0f, 0.0f, 0.0f, 1000.0f, 1000.0f, 1000.0f);

	/*heightMap.OnInit(m_pd3dDevice, &m_Camera);*/

	/*m_Terrain.OnInit(m_pd3dDevice, &D3DXVECTOR3(1.0f, 1.0f, 1.0f), "map129.bmp", "Heightblend.png", &m_Camera);*/
	// 눈
	//BoundingBox boundingBox;
	//boundingBox.SetVector(D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(-2.0f, -2.0f, -0.0f)); // z축을 조정할 경우 y축이 조정됨... y축은 그 반대.

	//snow.InitSnow(&boundingBox, 500);
	//snow.OnInit(m_pd3dDevice, "snowflake.dds");

	// 불
	D3DXVECTOR3 origin(0.0f, 0.0f, 1.0f);
	firework.InitFirework(&origin, 1000);
	firework.OnInit(m_pd3dDevice, "flare.bmp");

	// 총
	Gun.InitGun(&m_Camera);
	Gun.OnInit(m_pd3dDevice, "flare_alpha.dds");

	// 애니메이션------------------------------------------
	TestAni.OnInit(m_pd3dDevice, (char*)"./SkinnedMesh/tiny.x");
	//-----------------------------------------------------
	//m_Test.OnInit(m_pd3dDevice);

	player.OnInit(m_pd3dDevice, NULL, NULL, m_hWnd);
	OtherPlayer.OnInit(m_pd3dDevice, NULL);

	D3DXCreateFont(m_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"System", &m_Font);

	m_Text.OnInit(m_pd3dDevice, 20);

	return S_OK;
}

void TCPIPGame::OnRender()
{
	//m_pd3dDevice->SetLight(0, &light);
	//m_pd3dDevice->LightEnable(0, true);
	//m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffff);

	// Object Render ------------------------

	m_SkyBox.OnRender(*player.GetCamera());


	// TestAni.OnRender();

	//---------------------------------------

	// Map Render -----------------------------------

	//Map.OnRender(m_Camera);

	WhiteTile.OnRender(*player.GetCamera());

	player.OnRender();
	OtherPlayer.OnRender(player.GetCamera(), &light);
	//heightMap.OnRender();

	/*m_Terrain.OnRender();*/
	//------------------------------------------------

	// Particle Render--------------------------------

	// firework.OnRender();

	/*snow.OnRender();*/

	Gun.OnRender();

	//-------------------------------------------------
	//fps 생성
	char string[100];
	RECT rt = { 10, 10, 0, 0 };
	sprintf_s(string, "FPS: %f", m_nFPS);
	m_Font->DrawTextA(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	m_nFPSCount++;

	m_Text.OnRender(30, 30, "테스트 스트링입니다.", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//rt = { 10, 25, 0, 0 };
	//sprintf_s(string, "Object Pos - x: %f, y: %f, z:%f", Pickax.GetPos().x, Pickax.GetPos().y, Pickax.GetPos().z);
	//m_Font->DrawTextA(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	
}

void TCPIPGame::OnUpdate()
{

	// timeDelta
	//timeDelta += 0.0001f;

	//FPS 계산
	double dwCurTime = GetTickCount();
	static double dwOldTime = GetTickCount();
	static double dwAccumulateTime = 0;
	m_dwElapsedTime = dwCurTime - dwOldTime;
	dwOldTime = dwCurTime;

	dwAccumulateTime += m_dwElapsedTime;
	if (dwAccumulateTime >= 1000.0f)
	{
		dwAccumulateTime = 0.0f;
		m_nFPS = m_nFPSCount;
		m_nFPSCount = 0.0f;
	}
	
	// player
	player.GetCamera()->OnUpdate();
	OtherPlayer.OnUpdate();

	// TestAni.OnUpdate((float)m_dwElapsedTime);
	//Map.OnUpdate();

	WhiteTile.OnUpdate();

	//heightMap.OnUpdate();

	//snow.OnUpdate(timeDelta);

	Gun.OnUpdate(timeDelta, m_Camera.GetAt());
}
void TCPIPGame::OnRelease()
{
	m_Text.OnRelease();

	m_Font->Release();

	Gun.OnRelease();

	firework.OnRelease();

	snow.OnRelease();

	m_Terrain.OnRelease();

	heightMap.OnRelease();

	Map.OnRelease();

	WhiteTile.OnRelease();

	TestObject.OnRelease();

	SkinningObject.OnRelease();

	//Test
	TestAni.OnRelease();

	player.OnRelease();
	OtherPlayer.OnRelease();
	//-----


	//m_Test.OnRelease();

	m_SkyBox.OnRelease();

}

// Mouse Event

void TCPIPGame::OnLeftMouseClick()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);
	D3DXMATRIX matProj;
	m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DVIEWPORT9 vp;
	m_pd3dDevice->GetViewport(&vp);

	D3DXVECTOR3 v;

	D3DXMATRIX matView;
	m_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &matView);

	v.x = (((2.0f*point.x) / vp.Width) - 1.0f - matProj._31) / matProj._11;
	v.y = -(((2.0f*point.y) / vp.Height) - 1.0f - matProj._32)  / matProj._22;
	v.z = 1.0f;

	PickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	PickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	PickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;

	PickRayOrig.x = m._41;
	PickRayOrig.y = m._42;
	PickRayOrig.z = m._43;
	
	// 행렬 변환
	//D3DXVec3TransformCoord(&PickRayOrig, &PickRayOrig, &m);
	//D3DXVec3TransformNormal(&PickRayDir, &PickRayDir, &m);
	//D3DXVec3Normalize(&PickRayDir, &PickRayDir);
	
	result = WhiteTile.IntersectRay(&PickRayDir, &PickRayOrig);

	sprintf_s(RayMsg, "Ray - x : %f, y : %f, z : %f", PickRayDir.x, PickRayDir.y, PickRayDir.z);
	sprintf_s(OrigMsg, "Orig - x : %f, y: %f, z : %f", PickRayOrig.x, PickRayOrig.y, PickRayOrig.z);
	if(result.first)
	{
		sprintf_s(PickMsg, "resultRay - x : %f, y : %f, z : %f", result.second.x, result.second.y, result.second.z);
	}
}