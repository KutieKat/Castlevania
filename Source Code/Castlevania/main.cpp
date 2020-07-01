#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Game.h"
#include "Utilities/Color.h"
#include "Scenes/PlayScene.h"
#include "resource.h"

CGame* game;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Update(DWORD dt)
{
	CGame::GetInstance()->HandleEnding();
	CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->Update(dt);
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, CColor::FromRgb(0, 0, 0));

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowClassName, LPCWSTR windowTitle, int screenWidth, int screenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

	float windowX = (rc.right / 2) - (screenWidth / 2);
	float windowY = (rc.bottom / 2) - (screenHeight / 2);

	HWND hWnd =
		CreateWindow(
			windowClassName,
			windowTitle,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			windowX,
			windowY,
			screenWidth,
			screenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			if (!game->GetSceneManager()->GetCurrentScene()->HardPaused())
			{
				game->GetTimer()->Tick();
			}
			
			if (dynamic_cast<CPlayScene*>(game->GetSceneManager()->GetCurrentScene()))
			{
				CSimon* simon = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetSceneManager()->GetCurrentScene())->GetPlayer();

				if (simon->GetState() != SIMON_STATE_DELAY && simon->GetState() != SIMON_STATE_DIE)
				{
					game->GetInputManager()->ProcessKeyboard();
				}
				else
				{
					if (((CPlayScene*)game->GetSceneManager()->GetCurrentScene())->ShowingGameOverBoard())
					{
						game->GetInputManager()->ProcessKeyboard();
					}
				}
			}
			else
			{
				game->GetInputManager()->ProcessKeyboard();
			}

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, L"Castlevania", L"Castlevania", SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);
	game->GetSettingManager()->LoadFromFile("Settings.xml");
	game->GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	game->GetSceneManager()->Load("Resources\\Scenes\\Scenes.xml");

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}