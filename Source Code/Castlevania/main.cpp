#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Animations/AnimationManager.h"
#include "Animations/Animation.h"
#include "Effects/Effect.h"
#include "Game.h"
#include "Models/Characters/Players/Simon.h"
#include "Models/GameObject.h"
#include "Models/Items/BigHeart.h"
#include "Models/Items/Dagger.h"
#include "Models/Items/EasterEgg.h"
#include "Models/Items/MoneyBag.h"
#include "Models/Items/MorningStar.h"
#include "Models/Misc/BigCandle.h"
#include "Models/Misc/Blackboard.h"
#include "Models/Misc/Brick.h"
#include "Models/Misc/Door.h"
#include "Models/Misc/DoorWall.h"
#include "Sprites/SpriteManager.h"
#include "Textures/TextureManager.h"
#include "TileMap/TileMap.h"
#include "Utilities/Color.h"
#include "Utilities/Constants.h"
#include "Utilities/Debug.h"
#include "resource.h"

CGame* game;
CSimon* simon;
CBlackboard* blackboard;

vector<LPGAMEOBJECT> objects;
vector<CEffect*> effects;
CTileMap* tileMap;

class CSampleKeyHander : public IKeyEventHandler
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
	case DIK_Z:
		if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK || (simon->TouchingGround() == false && simon->GetState() == SIMON_STATE_STAND_AND_ATTACK))
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK || (simon->Sitting() == true && simon->GetState() == SIMON_STATE_SIT_AND_ATTACK))
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_WALK || simon->GetState() == SIMON_STATE_JUMP)
		{
			simon->SetState(SIMON_STATE_STAND_AND_ATTACK);
		}
		else if (simon->GetState() == SIMON_STATE_SIT)
		{
			simon->SetState(SIMON_STATE_SIT_AND_ATTACK);
		}

		break;

	case DIK_X:
		if (simon->Sitting())
		{
			return;
		}
		else
		{
			simon->SetState(SIMON_STATE_JUMP);
		}

		break;

	default:
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int keyCode)
{
	switch (keyCode)
	{
	case DIK_DOWN:
		simon->SetState(SIMON_STATE_IDLE);
		break;
	}
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	if (simon->GetState() == SIMON_STATE_JUMP && simon->TouchingGround() == false)
	{
		return;
	}

	if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && simon->animations[simon->GetAnimationToRender()]->Over() == false)
	{
		return;
	}

	if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK && simon->animations[simon->GetAnimationToRender()]->Over() == false)
	{
		return;
	}

	if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
	{
		simon->SetState(SIMON_STATE_SIT);
	}

	if (game->GetInputManager()->IsKeyDown(DIK_RIGHT))
	{
		simon->SetDirection(Direction::Right);

		if (!simon->Sitting())
		{
			simon->SetState(SIMON_STATE_WALK);
		}
	}
	else if (game->GetInputManager()->IsKeyDown(DIK_LEFT))
	{
		simon->SetDirection(Direction::Left);

		if (!simon->Sitting())
		{
			simon->SetState(SIMON_STATE_WALK);
		}
	}
	else if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
	{
		simon->SetState(SIMON_STATE_SIT);
	}
	else
	{
		if (game->GetTimer()->GetRemainingTime() > 0)
		{
			simon->SetState(SIMON_STATE_IDLE);
		}
		else
		{
			simon->SetState(SIMON_STATE_DIE);
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
	sprites->LoadFromFile("Resources\\Weapons\\Whip.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Items\\MoneyBag.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Items\\BigHeart.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Items\\Dagger.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Items\\MorningStar.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Others\\Transparency.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Others\\Blackboard.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Others\\HealthVolume.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Effects\\OneThousand.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Effects\\Flash.SpriteSheet.xml");
	sprites->LoadFromFile("Resources\\Ground\\DoorWall.SpriteSheet.xml");

	CAnimationManager* animations = CAnimationManager::GetInstance();
	animations->LoadFromFile("Resources\\Characters\\Players\\Simon.Animations.xml");
	animations->LoadFromFile("Resources\\Ground\\Brick.Animations.xml");
	animations->LoadFromFile("Resources\\Ground\\BigCandle.Animations.xml");
	animations->LoadFromFile("Resources\\Weapons\\Whip.Animations.xml");
	animations->LoadFromFile("Resources\\Items\\MoneyBag.Animations.xml");
	animations->LoadFromFile("Resources\\Items\\BigHeart.Animations.xml");
	animations->LoadFromFile("Resources\\Items\\Dagger.Animations.xml");
	animations->LoadFromFile("Resources\\Items\\MorningStar.Animations.xml");
	animations->LoadFromFile("Resources\\Others\\Transparency.Animations.xml");
	animations->LoadFromFile("Resources\\Others\\Blackboard.Animations.xml");
	animations->LoadFromFile("Resources\\Others\\HealthVolume.Animations.xml");
	animations->LoadFromFile("Resources\\Effects\\OneThousand.Animations.xml");
	animations->LoadFromFile("Resources\\Effects\\Flash.Animations.xml");
	animations->LoadFromFile("Resources\\Ground\\DoorWall.Animations.xml");

	tileMap = new CTileMap("Resources\\Maps\\Scene1.Map.xml", L"Resources\\Maps\\Scene1.png");

	CEffect* oneThousand = new CEffect();
	oneThousand->AddAnimation("one_thousand");
	oneThousand->SetTimeout(300);
	effects.push_back(oneThousand);

	CMoneyBag* moneyBag = new CMoneyBag();
	moneyBag->AddAnimation("money_bag");
	moneyBag->SetEndingEffect(oneThousand);
	moneyBag->SetScore(1000);
	moneyBag->SetPosition(1256, 367);
	objects.push_back(moneyBag);

	CEasterEgg* easterEgg = new CEasterEgg();
	easterEgg->AddAnimation("transparency");
	easterEgg->SetPosition(1420, 334);
	easterEgg->SetHiddenItem(moneyBag);
	objects.push_back(easterEgg);

	for (int i = 0; i < 48; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(i * 32.0f, 366);
		objects.push_back(brick);
	}

	for (int i = 0; i < 5; i++)
	{
		CEffect* flash = new CEffect();
		flash->AddAnimation("flash");
		flash->SetTimeout(200);
		effects.push_back(flash);

		CBigCandle* bigCandle = new CBigCandle();
		bigCandle->AddAnimation("big_candle");
		bigCandle->SetEndingEffect(flash);

		if (i == 0 || i == 3)
		{
			CBigHeart* bigHeart = new CBigHeart();
			bigHeart->AddAnimation("big_heart");

			if (i == 0)
			{
				bigCandle->SetPosition(180, 304);
				bigHeart->SetPosition(180, 304);
			}
			else
			{
				bigCandle->SetPosition(950, 304);
				bigHeart->SetPosition(950, 304);
			}

			objects.push_back(bigHeart);

			bigCandle->SetHiddenItem(bigHeart);
		}
		else if (i == 1 || i == 2)
		{
			CMorningStar* morningStar = new CMorningStar();
			morningStar->AddAnimation("morning_star");

			if (i == 1)
			{
				bigCandle->SetPosition(430, 304);
				morningStar->SetPosition(430, 304);
			}
			else
			{
				bigCandle->SetPosition(690, 304);
				morningStar->SetPosition(690, 304);
			}

			objects.push_back(morningStar);

			bigCandle->SetHiddenItem(morningStar);
		}
		else
		{
			CDagger* dagger = new CDagger();
			dagger->AddAnimation("dagger");
			bigCandle->SetPosition(1200, 304);
			dagger->SetPosition(1200, 304);
			objects.push_back(dagger);

			bigCandle->SetHiddenItem(dagger);
		}

		objects.push_back(bigCandle);
	}

	simon = new CSimon();
	simon->AddAnimation("simon_idle_left");
	simon->AddAnimation("simon_walk_left");
	simon->AddAnimation("simon_sit_left");
	simon->AddAnimation("simon_jump_left");
	simon->AddAnimation("simon_sit_left_and_attack");
	simon->AddAnimation("simon_stand_left_and_attack");
	simon->AddAnimation("simon_delay_left");
	simon->AddAnimation("simon_die_left");

	simon->AddAnimation("simon_idle_right");
	simon->AddAnimation("simon_walk_right");
	simon->AddAnimation("simon_sit_right");
	simon->AddAnimation("simon_jump_right");
	simon->AddAnimation("simon_sit_right_and_attack");
	simon->AddAnimation("simon_stand_right_and_attack");
	simon->AddAnimation("simon_delay_right");
	simon->AddAnimation("simon_die_right");

	simon->SetPosition(65.0f, 100.0f);
	objects.push_back(simon);
	
	CDoorWall* doorWall = new CDoorWall();
	doorWall->AddAnimation("door_wall");
	doorWall->SetPosition(1408, 233);
	doorWall->SetVisibility(Visibility::Hidden);
	objects.push_back(doorWall);

	CDoor* door = new CDoor();
	door->AddAnimation("transparency");
	door->SetPosition(1376, 356);
	door->SetDoorWall(doorWall);
	objects.push_back(door);

	blackboard = new CBlackboard(simon);
	blackboard->AddAnimation("blackboard");
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
		if (objects[i]->GetVisibility() == Visibility::Visible)
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
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetVisibility() == Visibility::Visible)
		{
			objects[i]->Update(dt, &coObjects);
		}
	}

	// Update timer
	game->GetTimer()->Tick();

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
	else if (currentPlayerX + SCREEN_WIDTH / 2 >= tileMap->GetWidth()) {
		float left, top, right, bottom;

		simon->GetBoundingBox(left, top, right, bottom);
		cx = tileMap->GetWidth() - SCREEN_WIDTH;

		float playerBoundingBoxWidth = right - left;

		if (currentPlayerX >= tileMap->GetWidth() - playerBoundingBoxWidth) {
			currentPlayerX = tileMap->GetWidth() - playerBoundingBoxWidth;
			simon->SetPosition(currentPlayerX, currentPlayerY);
		}
	}
	else {
		cx -= SCREEN_WIDTH / 2;
	}

	cy -= SCREEN_HEIGHT / 2;

	// Update blackboard position
	blackboard->Update(tileMap);

	CGame::GetInstance()->SetCamPos(cx, 0.0f);
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
		d3ddv->ColorFill(bb, NULL, CColor::FromRgb(0, 0, 0));

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		// TileMap
		float camX, camY;
		game->GetCamPos(camX, camY);

		tileMap->Render(camX, camY, camX + SCREEN_WIDTH, camY + SCREEN_HEIGHT);

		// Effects
		for (int i = 0; i < effects.size(); i++)
		{
			if (effects[i]->GetStartTime() != -1)
			{
				effects[i]->Render();
			}
		}

		// Objects
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetVisibility() == Visibility::Visible)
			{
				objects[i]->Render();
			}
		}

		// Blackboard
		blackboard->Render();

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
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

	float windowX = (rc.right / 2) - (SCREEN_WIDTH / 2);
	float windowY = (rc.bottom / 2) - (SCREEN_HEIGHT / 2);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			windowX,
			windowY,
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

			if (simon->GetState() != SIMON_STATE_AUTO_WALK && simon->GetState() != SIMON_STATE_DELAY && simon->GetState() != SIMON_STATE_DIE)
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
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	keyHandler = new CSampleKeyHander();

	game = CGame::GetInstance();
	game->Init(hWnd, keyHandler);

	game->GetTimer()->SetTime(300);
	game->GetTimer()->Start();

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}