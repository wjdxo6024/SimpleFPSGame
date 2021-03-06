// D3DX9ProjectA.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "D3DX9ProjectA.h"
#include "TCPIPGame.h"
#include "D3D9Engine.h"
#include "Picking.h"
#include "CD3DApp.h"
// Scene
#include "InitScene.h"
#include "GameScene.h"

#include <d3d9.h>
#include <d3dx9math.h>
#include <cstdio>
#include <thread>
#include <ctime>
#include <locale>

#define MAX_LOADSTRING 100
// 전역 변수:
HWND g_hWnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

D3D9Engine D3DEngine;
InitScene g_InitScene;
GameScene g_GameScene;
LoadScene g_LoadScene;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D3DX9PROJECTA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	// 한글 사용
	

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	// 게임 초기화 - 로딩 씬 출력과 엔진 초기화
	{
		D3DEngine.InitD3D(g_hWnd);

		g_InitScene.OnInit(D3DEngine.GetDevice(), g_hWnd);
		time_t start, end;
		start = time(NULL);
		end = time(NULL);
		thread InitGameScene(&GameScene::OnInit, &g_GameScene, D3DEngine.GetDevice(), g_hWnd);
		
		thread LoadingGameScene([&]()
		{ 
			D3DEngine.BeginScene();
			g_InitScene.OnRender();
			D3DEngine.EndScene();}
		);
		InitGameScene.join();
		LoadingGameScene.join();
		/*while (InitGameScene.joinable())
		{
			end = time(NULL);
			D3DEngine.BeginScene();
			g_InitScene.OnRender();
			D3DEngine.EndScene();
		}*/
		g_InitScene.OnRelease();
	}

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D3DX9PROJECTA));

    MSG msg;

    // 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			D3DEngine.Update();

			// 렌더링
			D3DEngine.BeginScene();

			g_GameScene.OnRender();

			D3DEngine.EndScene();
		}
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DX9PROJECTA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_BORDER,
      CW_USEDEFAULT, 0, 1024, 768, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_GameScene.OnUpdate(message, wParam, lParam);
    switch (message)
    {
	case WM_KEYDOWN:
	{

	}
	break;
    case WM_COMMAND:
        {
            
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			/*D3DApp.Render();*/
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		D3DEngine.Cleanup();
		g_GameScene.OnRelease();
        PostQuitMessage(0);
        break;
    default:
		switch (lParam)
		{
		case FD_READ:
			break;
		}

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
