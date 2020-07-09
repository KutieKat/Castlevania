//#include <windows.h>
//#include <d3d9.h>
//#include <d3dx9.h>

#include "Game.h"
#include "Utilities/Debug.h"
#include "Libraries/TinyXML/tinyxml.h"
#include "Scenes/PlayScene.h"
#include "Scenes/IntroScene.h"
#include "Scenes/CutScene.h"
//#include "resource.h"

CGame* CGame::instance = nullptr;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
//void CGame::_Init(HINSTANCE hInstance, int nCmdShow)
//{
//	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
//
//	//this->hWnd = hWnd;
//	this->hWnd = CreateGameWindow(hInstance, nCmdShow);
//
//	D3DPRESENT_PARAMETERS d3dpp;
//
//	ZeroMemory(&d3dpp, sizeof(d3dpp));
//
//	d3dpp.Windowed = TRUE;
//	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
//	d3dpp.BackBufferCount = 1;
//
//	RECT r;
//	GetClientRect(hWnd, &r);	// retrieve Window width & height 
//
//	d3dpp.BackBufferHeight = r.bottom + 1;
//	d3dpp.BackBufferWidth = r.right + 1;
//
//	d3d->CreateDevice(
//		D3DADAPTER_DEFAULT,
//		D3DDEVTYPE_HAL,
//		hWnd,
//		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//		&d3dpp,
//		&d3ddv);
//
//	if (d3ddv == NULL)
//	{
//		CDebug::Error("CreateDevice failed!", "Game.cpp");
//		return;
//	}
//
//	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
//
//	// Initialize sprite helper from Direct3DX helper library
//	D3DXCreateSprite(d3ddv, &spriteHandler);
//
//	// Setting manager
//	settingManager = CSettingManager::GetInstance();
//	settingManager->LoadFromFile("Settings.xml");
//
//	// Initialize input manager
//	inputManager = CInputManager::GetInstance();
//	inputManager->Init(hWnd);
//
//	// Timer
//	timer = new CTimer();
//
//	// Camera
//	camera = CCamera::GetInstance();
//	camera->Init();
//	camera->SetSize(settingManager->GetIntValue("SCREEN_WIDTH"), settingManager->GetIntValue("SCREEN_HEIGHT"));
//
//	// Player data
//	playerData = CPlayerData::GetInstance();
//	playerData->Init();
//
//	// Boss data
//	bossData = CBossData::GetInstance();
//	bossData->Init();
//
//	// Scene manager
//	sceneManager = CSceneManager::GetInstance();
//	sceneManager->Load("Resources\\Scenes\\Scenes.xml");
//
//	// Sound manager
//	soundManager = CGameSoundManager::GetInstance();
//	soundManager->Init();
//
//	// Game ending
//	ended = false;
//
//	healingCounter = 0;
//	timeScoreCounter = 0;
//	heartsScoreCounter = 0;
//	switchSceneCounter = 0;
//
//	CDebug::Info("Initialize game successfully!", "Game.cpp");
//}

//int CGame::_Run()
//{
//	MSG msg;
//	int done = 0;
//	DWORD frameStart = GetTickCount();
//	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
//
//	while (!done)
//	{
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			if (msg.message == WM_QUIT) done = 1;
//
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//
//		DWORD now = GetTickCount();
//
//		// dt: the time between (beginning of last frame) and now
//		// this frame: the frame we are about to render
//		DWORD dt = now - frameStart;
//
//		if (dt >= tickPerFrame)
//		{
//			frameStart = now;
//
//			if (!sceneManager->GetCurrentScene()->HardPaused())
//			{
//				timer->Tick();
//			}
//
//			if (dynamic_cast<CPlayScene*>(sceneManager->GetCurrentScene()))
//			{
//				CSimon* simon = dynamic_cast<CPlayScene*>(sceneManager->GetCurrentScene())->GetPlayer();
//
//				if (simon->GetState() != SIMON_STATE_DELAY && simon->GetState() != SIMON_STATE_DIE)
//				{
//					inputManager->ProcessKeyboard();
//				}
//				else
//				{
//					if (((CPlayScene*)sceneManager->GetCurrentScene())->ShowingGameOverBoard())
//					{
//						inputManager->ProcessKeyboard();
//					}
//				}
//			}
//			else
//			{
//				inputManager->ProcessKeyboard();
//			}
//
//			Update(dt);
//			Render();
//		}
//		else
//			Sleep(tickPerFrame - dt);
//	}
//
//	return 1;
//}

void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		CDebug::Error("CreateDevice failed!", "Game.cpp");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	// Initialize input manager
	inputManager = CInputManager::GetInstance();
	inputManager->Init(hWnd);

	// Timer
	timer = new CTimer();

	// Camera
	camera = CCamera::GetInstance();
	camera->Init();

	// Player data
	playerData = CPlayerData::GetInstance();
	playerData->Init();

	// Boss data
	bossData = CBossData::GetInstance();
	bossData->Init();

	// Scene manager
	sceneManager = CSceneManager::GetInstance();

	// Sound manager
	soundManager = CGameSoundManager::GetInstance();
	soundManager->Init();

	// Game ending
	ended = false;

	healingCounter = 0;
	timeScoreCounter = 0;
	heartsScoreCounter = 0;
	switchSceneCounter = 0;

	CDebug::Info("Initialize game successfully!", "Game.cpp");
}

void CGame::SetKeyHandler(IKeyEventHandler* keyHandler)
{
	this->inputManager->SetKeyHandler(keyHandler);
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw
*/
void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{

	//D3DXVECTOR3 p(floor(x), floor(y), 0); // https://docs.microsoft.com/vi-vn/windows/desktop/direct3d9/directly-mapping-texels-to-pixels
	// Try removing floor() to see blurry Mario
		//D3DXVECTOR3 p(floor(x - camX), floor(y - camY), 0);
	D3DXVECTOR3 p(floor(x - this->camera->GetLeft()), floor(y - this->camera->GetTop()), 0);

	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

bool CGame::HaveCollision(CGameObject* object1, CGameObject* object2)
{
	float l1, t1, r1, b1;
	float l2, t2, r2, b2;

	object1->GetBoundingBox(l1, t1, r1, b1);
	object2->GetBoundingBox(l2, t2, r2, b2);

	float left = l1 - r2;
	float top = b1 - t2;
	float right = r1 - l2;
	float bottom = t1 - b2;

	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

bool CGame::Ended()
{
	return ended;
}

void CGame::SetPauseStartingTime(DWORD time)
{
	pauseStartingTime = time;
}

void CGame::SetPauseEndingTime(DWORD time)
{
	pauseEndingTime = time;
}

void CGame::End()
{
	ended = true;
}

void CGame::HandleEnding()
{
	if (ended)
	{
		sceneManager->GetCurrentScene()->HardPause(false);
		timer->Pause();

		if (playerData->GetHealthVolumes() != HEALTH_BAR_MAX_VOLUMES)
		{
			healingCounter += 1;

			if (healingCounter % 10 == 0)
			{
				playerData->AddHealthVolumes(1);
			}
		}
		else
		{
			timeScoreCounter += 1;

			if (timer->GetRemainingTime() != 0)
			{
				if (timeScoreCounter % 3 == 0)
				{
					soundManager->Play("adding_time_score");
					timer->Decrease();
					playerData->AddScore(settingManager->GetIntValue("TIME_SCORE"));
				}
			}
			else
			{
				heartsScoreCounter += 1;

				if (playerData->GetHearts() > 0)
				{
					if (heartsScoreCounter % 10 == 0)
					{
						soundManager->Play("adding_hearts_score");
						playerData->DecreaseHearts(1);
						playerData->AddScore(settingManager->GetIntValue("HEART_SCORE"));
					}
				}
				else
				{
					switchSceneCounter += 1;

					if (switchSceneCounter % 500 == 0)
					{
						sceneManager->SwitchSceneByIndex(sceneManager->GetNextSceneIndex());
					}
				}
			}
		}
	}
}

DWORD CGame::GetPauseDeltaTime()
{
	DWORD deltaTime = pauseEndingTime - pauseStartingTime;

	if (pauseEndingTime != -1 && pauseStartingTime != -1 && deltaTime < 0)
	{
		return 0;
	}

	return deltaTime;
}

CInputManager* CGame::GetInputManager()
{
	return this->inputManager;
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

/*
	SweptAABB
*/
void CGame::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float &t, float &nx, float &ny)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

//HWND CGame::CreateGameWindow(HINSTANCE hInstance, int nCmdShow)
//{
//	WNDCLASSEX wc;
//	wc.cbSize = sizeof(WNDCLASSEX);
//
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	wc.hInstance = hInstance;
//
//	wc.lpfnWndProc = (WNDPROC)WinProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = L"Castlevania";
//	wc.hIconSm = NULL;
//
//	RegisterClassEx(&wc);
//
//	RECT rc;
//	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
//
//	int screenWidth = settingManager->GetIntValue("SCREEN_WIDTH");
//	int screenHeight = settingManager->GetIntValue("SCREEN_HEIGHT");
//
//	float windowX = (rc.right / 2) - (screenWidth / 2);
//	float windowY = (rc.bottom / 2) - (screenHeight / 2);
//
//	HWND hWnd =
//		CreateWindow(
//			L"Castlevania",
//			L"Castlevania",
//			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
//			windowX,
//			windowY,
//			screenWidth,
//			screenHeight,
//			NULL,
//			NULL,
//			hInstance,
//			NULL);
//
//	if (!hWnd)
//	{
//		OutputDebugString(L"[ERROR] CreateWindow failed");
//		DWORD ErrCode = GetLastError();
//		return FALSE;
//	}
//
//	ShowWindow(hWnd, nCmdShow);
//	UpdateWindow(hWnd);
//
//	return hWnd;
//}

//LRESULT CGame::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message) {
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//
//	return 0;
//}

LPDIRECT3DDEVICE9 CGame::GetDirect3DDevice()
{
	return this->d3ddv;
}

LPDIRECT3DSURFACE9 CGame::GetBackBuffer()
{
	return this->backBuffer;
}

LPD3DXSPRITE CGame::GetSpriteHandler()
{
	return this->spriteHandler;
}

HWND CGame::GetHandler()
{
	return hWnd;
}

CTimer* CGame::GetTimer()
{
	return this->timer;
}

CCamera* CGame::GetCamera()
{
	return this->camera;
}

CPlayerData* CGame::GetPlayerData()
{
	return playerData;
}

CBossData* CGame::GetBossData()
{
	return bossData;
}

CSceneManager* CGame::GetSceneManager()
{
	return sceneManager;
}

CGameSoundManager* CGame::GetSoundManager()
{
	return soundManager;
}

CGame* CGame::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CGame();
	}

	return instance;
}

//void CGame::Render()
//{
//	LPDIRECT3DDEVICE9 d3ddv = GetDirect3DDevice();
//	LPDIRECT3DSURFACE9 bb = GetBackBuffer();
//	LPD3DXSPRITE spriteHandler = GetSpriteHandler();
//
//	if (d3ddv->BeginScene())
//	{
//		// Clear back buffer with a color
//		d3ddv->ColorFill(bb, NULL, CColor::FromRgb(0, 0, 0));
//
//		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
//
//		sceneManager->GetCurrentScene()->Render();
//
//		spriteHandler->End();
//		d3ddv->EndScene();
//	}
//
//	// Display back buffer content to the screen
//	d3ddv->Present(NULL, NULL, NULL, NULL);
//}

//void CGame::Update(DWORD dt)
//{
//	HandleEnding();
//	sceneManager->GetCurrentScene()->Update(dt);
//}

void CGame::Reset()
{
	ended = false;
	playerData->Reset();
	sceneManager->Reset();
	timer->SetTime(settingManager->GetIntValue("DEFAULT_GAME_TIME"));
}
