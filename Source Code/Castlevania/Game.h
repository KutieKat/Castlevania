#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Input/InputManager.h"
#include "Models/GameObject.h"
#include "Utilities/Timer.h"
#include "Camera/Camera.h"
#include "Scenes/Scene.h"
#include "Textures/TextureManager.h"
#include "Models/Characters/Players/PlayerData.h"
#include "Models/Characters/Bosses/BossData.h"
#include "Scenes/SceneManager.h"
#include "Sounds/GameSoundManager.h"

class CGame
{
	static CGame* instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	CInputManager* inputManager;
	CTimer* timer;
	CCamera* camera;
	CPlayerData* playerData;
	CBossData* bossData;
	CSceneManager* sceneManager;
	CGameSoundManager* soundManager;

	DWORD pauseStartingTime = -1;
	DWORD pauseEndingTime = -1;

	int healingCounter;
	int timeScoreCounter;
	int heartsScoreCounter;
	int switchSceneCounter;

	bool ended;

public:
	//void _Init(HINSTANCE hInstance, int nCmdShow);
	//int _Run();

	void Init(HWND hWnd);
	void SetKeyHandler(IKeyEventHandler* keyHandler);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	bool HaveCollision(CGameObject* object1, CGameObject* object2);
	bool Ended();

	void SetPauseStartingTime(DWORD time);
	void SetPauseEndingTime(DWORD time);
	void End();
	void HandleEnding();

	DWORD GetPauseDeltaTime();

	CInputManager* GetInputManager();

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float &t,
		float &nx,
		float &ny);

	//HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow);
	//static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	LPDIRECT3DDEVICE9 GetDirect3DDevice();
	LPDIRECT3DSURFACE9 GetBackBuffer();
	LPD3DXSPRITE GetSpriteHandler();

	HWND GetHandler();

	CTimer* GetTimer();
	CCamera* GetCamera();
	CPlayerData* GetPlayerData();
	CBossData* GetBossData();
	CSceneManager* GetSceneManager();
	CGameSoundManager* GetSoundManager();

	static CGame* GetInstance();

	//void Render();
	//void Update(DWORD dt);

	void Reset();

	~CGame();
};