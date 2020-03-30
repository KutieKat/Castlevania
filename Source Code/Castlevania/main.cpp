#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Animations/AnimationManager.h"
#include "Animations/Animation.h"
#include "Game.h"
#include "Models/GameObject.h"
#include "Models/Characters/Players/Simon.h"
#include "Models/Misc/BigCandle.h"
#include "Models/Misc/Brick.h"
#include "Sprites/SpriteManager.h"
#include "Textures/TextureManager.h"
#include "Utilities/Color.h"
#include "Utilities/Constants.h"
#include "Utilities/Debug.h"

CGame* game;
CSimon* simon;

vector<LPGAMEOBJECT> objects;

class CSampleKeyHander: public IKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler; 

void CSampleKeyHander::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case DIK_DOWN:
		simon->SetState(SIMON_STATE_SITTING);
		break;

	case DIK_Z:
		if (simon->sitting)
		{
			simon->SetState(SIMON_STATE_SITTING_AND_ATTACKING);
		}
		else
		{
			simon->SetState(SIMON_STATE_STANDING_AND_ATTACKING);
		}
		break;

	case DIK_X:
		if (!simon->sitting && !simon->jumping)
		{
			simon->SetState(SIMON_STATE_JUMPING);
		}

		break;
	}
}


void CSampleKeyHander::OnKeyUp(int keyCode)
{
	switch (keyCode)
	{
	case DIK_DOWN:
		if (simon->attacking)
		{
			simon->SetState(SIMON_STATE_STANDING_AND_ATTACKING);
		}
		else
		{
			simon->SetState(SIMON_STATE_IDLE);
		}

		break;

	case DIK_Z:
		simon->SetState(SIMON_STATE_IDLE);
		break;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	if (game->GetInputManager()->IsKeyDown(DIK_RIGHT))
	{
		if (simon->jumping || simon->attacking)
		{
			return;
		}
		else if (simon->sitting)
		{
			simon->nx = 1;
		}
		else
		{
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
		}
	}
	else if (game->GetInputManager()->IsKeyDown(DIK_LEFT))
	{
		if (simon->jumping || simon->attacking)
		{
			return;
		}
		else if (simon->sitting)
		{
			simon->nx = -1;
		}
		else
		{
			simon->SetState(SIMON_STATE_WALKING_LEFT);
		}
	}
	else if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
	{
		if (simon->jumping)
		{
			return;
		}
		else if (simon->attacking)
		{
			simon->SetState(SIMON_STATE_SITTING_AND_ATTACKING);
		}
		else
		{
			simon->SetState(SIMON_STATE_SITTING);
		}
	}
	else
	{
		if (!simon->jumping && !simon->attacking)
		{
			simon->SetState(SIMON_STATE_IDLE);
		}
	}
}

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

void LoadResources()
{
	CTextureManager* textures = CTextureManager::GetInstance();
	textures->LoadFromFile("Resources\\Textures.xml");

	CSpriteManager* sprites = CSpriteManager::GetInstance();
	sprites->LoadFromFile("Resources\\Characters\\Players\\Simon.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Ground\\Brick.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Ground\\BigCandle.SpriteSheet.xml");

	CAnimationManager* animations = CAnimationManager::GetInstance();
	animations->LoadFromFile("Resources\\Characters\\Players\\Simon.Animations.xml");
	animations->LoadFromFile("Resources\\Ground\\Brick.Animations.xml");
	animations->LoadFromFile("Resources\\Ground\\BigCandle.Animations.xml");

	for (int i = 0; i < 48; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(i * 32.0f, 170);
		objects.push_back(brick);
	}

	for (int i = 0; i < 5; i++)
	{
		CBigCandle* bigCandle = new CBigCandle();
		bigCandle->AddAnimation("big_candle");
		bigCandle->SetPosition(i == 0 ? 300 : (i + 1) * 300, 108);
		objects.push_back(bigCandle);
	}

	simon = new CSimon();
	simon->AddAnimation("simon_idle_left");
	simon->AddAnimation("simon_walk_left");
	simon->AddAnimation("simon_sit_left");
	simon->AddAnimation("simon_jump_left");
	simon->AddAnimation("simon_sit_left_and_hit");
	simon->AddAnimation("simon_stand_left_and_hit");

	simon->AddAnimation("simon_idle_right");
	simon->AddAnimation("simon_walk_right");
	simon->AddAnimation("simon_sit_right");
	simon->AddAnimation("simon_jump_right");
	simon->AddAnimation("simon_sit_right_and_hit");
	simon->AddAnimation("simon_stand_right_and_hit");

	simon->SetPosition(50.0f, 0);
	objects.push_back(simon);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CSimon*>(objects[i]))
		{
			continue;
		}
		else
		{
			coObjects.push_back(objects[i]);
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// Update camera to follow the player
	float cx, cy;
	simon->GetPosition(cx, cy);

	float currentPlayerX;
	float currentPlayerY;

	simon->GetPosition(currentPlayerX, currentPlayerY);

	if (currentPlayerX < SCREEN_WIDTH / 2) {
		cx = 0.0f;

		if (currentPlayerX <= 0) {
			currentPlayerX = 0.0f;
			simon->SetPosition(currentPlayerX, currentPlayerY);
		}
	}
	else if (currentPlayerX + SCREEN_WIDTH / 2 >= BACKGROUND_WIDTH) {
		cx = BACKGROUND_WIDTH - SCREEN_WIDTH;
		float playerBoundingBoxWidth = simon->GetBoundingBox().right - simon->GetBoundingBox().left;

		if (currentPlayerX >= BACKGROUND_WIDTH - playerBoundingBoxWidth) {
			currentPlayerX = BACKGROUND_WIDTH - playerBoundingBoxWidth;
			simon->SetPosition(currentPlayerX, currentPlayerY);
		}
	}
	else {
		cx -= SCREEN_WIDTH / 2;
	}

	cy -= SCREEN_HEIGHT / 2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
}

/*
	Render a frame 
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, CColor::FromRgb(255, 255, 200));

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
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

			game->GetInputManager()->ProcessKeyboard();
			
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
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	keyHandler = new CSampleKeyHander();

	game = CGame::GetInstance();
	game->Init(hWnd, keyHandler);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}