#include "SinglePlayScene.h"

bool SinglePlayScene::OnInit(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	timeDelta = 0.01f;
	m_pd3dDevice = pd3dDevice;
	m_hWnd = hWnd;

	m_Camera.OnInit(m_pd3dDevice, m_hWnd);
	//D3DXVECTOR3 Eye(0.0f, 400.0f, -5000.0f);
	//D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 LookAt(0.0f, 400.0f, 5000.0f);
	//m_Camera.SetMatrix(&Eye, &Up, &LookAt);
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;			/// 광원의 종류(점 광원,방향성 광원,스포트라이트)
	light.Diffuse.r = 1.0f;							/// 광원의 색깔과 밝기
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Position = D3DXVECTOR3(0.0f, 5000.0f, 100.0f);
	light.Direction = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	light.Range = 10000.0f;

	m_pd3dDevice->SetLight(0, &light);
	m_pd3dDevice->LightEnable(0, true);

	//m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	m_SkyBox.OnInit(m_pd3dDevice, m_Camera);
	
	// 모델들.
	// 맵
	Ground.OnInit("./Model/Plane.fbx", "./Texture/WhiteTile.jpg", m_pd3dDevice, m_Camera, light, 0.0f, -100.0f, 0.0f, 5000.0f, 5000.0f, 5000.0f);
	Wall.OnInit("./Model/wall_version2.fbx", "./Texture/wall_textures.png", m_pd3dDevice, m_Camera, light, 5000.0f, 0.0f, 0.0f, 50.0f, 5000.0f, 100.0f);

	// 바운딩 박스
	// MAX, MIN, POS, SIZE
	m_WallBoxs[0].OnInit(pd3dDevice, D3DXVECTOR3(5000, 200, -5000), D3DXVECTOR3(-5000, -100, -5100), D3DXVECTOR3(0, -100, -5050), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	// 총
	m_PlayerWeapons.OnInit(pd3dDevice, hWnd, "./Sound/pistolversion1.wav", "./Sound/pistol_reloading_version1.wav", "./Sound/pistol_dry_version1.wav", 20, 7);
	m_PlayerWeapons.SetLoadAmmo(7);
	m_PlayerWeapons.SetLeftAmmo(140);

	// 인터페이스
	m_PlayerInterface.OnInit(pd3dDevice, hWnd, &m_PlayerWeapons);
	m_GameChat.OnInit(pd3dDevice, hWnd);
	m_KillDeathEventList.OnInit(pd3dDevice, hWnd);
	// 플레이어 y = 400 에서 고정
	player.OnInit(m_pd3dDevice, NULL, NULL, m_hWnd);
	OtherPlayer.OnInit(m_pd3dDevice, NULL);

	D3DXCreateFont(m_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"System", &m_Font);

	m_Text.OnInit(m_pd3dDevice, 20);

	return S_OK;
}

bool SinglePlayScene::OnRender()
{
	//m_pd3dDevice->SetLight(0, &light);
	//m_pd3dDevice->LightEnable(0, true);
	//m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffff);

	// Object Render ------------------------

	m_SkyBox.OnRender(*player.GetCamera());

	// Test Bounding Box
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	// m_WallBoxs[0].OnRender(matWorld);
	
	// Map Render -----------------------------------
	Ground.OnRender(*player.GetCamera());
	Wall.OnRender(*player.GetCamera());

	//------------------------------------------------

	// 다른 사람 렌더링-----------------------------
	// OtherPlayer.OnRender(player.GetCamera(), &light);

	//-----------------------------------------

	// 인터페이스 렌더링--------------------------------
	m_PlayerInterface.OnRender();
	m_GameChat.OnRender();
	m_KillDeathEventList.OnRender();
	//------------------------------------------------
	// 스코어 렌더링

	//-------------------------------------------------

	//firework.Render();

	//fps 생성
	char string[100];
	RECT rt = { 10, 10, 0, 0 };
	sprintf_s(string, "FPS: %f", m_nFPS);
	m_Font->DrawTextA(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	m_nFPSCount++;
#ifdef _DEBUG
	sprintf_s(string, "Player Pos - x: %f, y: %f, z:%f", player.GetPlayerPos().x, player.GetPlayerPos().y, player.GetPlayerPos().z);
	m_Text.OnRender(10, 30, string, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
	//rt = { 10, 25, 0, 0 };
	//sprintf_s(string, "Object Pos - x: %f, y: %f, z:%f", Pickax.GetPos().x, Pickax.GetPos().y, Pickax.GetPos().z);
	//m_Font->DrawTextA(NULL, string, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	return true;
}

int SinglePlayScene::OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	// 모델 처리
	Ground.OnUpdate();
	Wall.OnUpdate();

	// Bounding Box
	m_WallBoxs[0].OnUpdate();
	// player
	if (m_GameChat.IsFocus()) // 채팅 사용 시
	{
	}
	else
	{
		player.GetCamera()->OnUpdate();
		player.OnUpdate(uMsg, wParam, lParam);
	}

	// other Player 네트워크에서 통신시 받을 것
	OtherPlayer.OnUpdate();

	// Bullet 처리

	// 시간 처리

	// 스코어 처리

	// 인터페이스 처리
	m_GameChat.OnUpdate(uMsg, wParam, lParam);
	m_PlayerInterface.OnUpdate(uMsg, wParam, lParam);
	m_KillDeathEventList.OnUpdate(uMsg, wParam, lParam, dwAccumulateTime);
	m_PlayerWeapons.OnUpdate(uMsg, wParam, lParam);

	HandleKeyBoard(uMsg, wParam, lParam);
	HandleMouse(uMsg, wParam, lParam);
	return 1;
}

bool SinglePlayScene::OnRelease()
{
	// Bounding Box
	m_WallBoxs[0].OnRelease();

	Ground.OnRelease();
	Wall.OnRelease();
	m_Text.OnRelease();

	m_Font->Release();


	player.OnRelease();
	OtherPlayer.OnRelease();
	//-----
	m_PlayerWeapons.OnRelease();
	m_GameChat.OnRelease();
	m_KillDeathEventList.OnRelease();
	m_PlayerInterface.OnRelease();
	//m_Test.OnRelease();

	m_SkyBox.OnRelease();
	return true;
}

int SinglePlayScene::HandleKeyBoard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 게임에 지장없는 키입력
	if (m_GameChat.IsFocus())
	{
		if (m_GameChat.IsFieldBlank())
		{
			if (wParam == VK_ESCAPE)
			{
				m_GameChat.ChattingFocus();
				return 0;
			}
		}
		m_GameChat.HandleKeyboard(uMsg, wParam, lParam);
	}
	else
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_RETURN: // 엔터시 채팅창 포커스
			{
				if (!m_GameChat.IsFocus())
				{
					m_GameChat.ChattingFocus();
				}
				break;
			}
			case 82: // R
			case 114:
				m_PlayerWeapons.Reload(m_Camera.GetEye().x, m_Camera.GetEye().y, m_Camera.GetEye().z);
				break;
			}
		}
		}
	}
	// 게임에 지장있는 키입력
	return 0;
}

int SinglePlayScene::HandleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_GameChat.IsFocus())
	{

	}
	else
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			m_PlayerWeapons.Shoot(m_Camera.GetEye().x, m_Camera.GetEye().y, m_Camera.GetEye().z);
			return 1;
		}
		}
	}
	return 0;
}

// private
bool SinglePlayScene::SetWallBoundingBox()
{
	return true;
}