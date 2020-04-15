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

	unordered_map<string, CScene*> scenes;
	string currentScene;

	int score;
	int hearts;
	int lives;
	int healthVolumes;

	string subWeaponType;

public:
	void Init(HWND hWnd);
	void SetKeyHandler(IKeyEventHandler* keyHandler);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	bool HaveCollision(CGameObject* object1, CGameObject* object2);

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

	LPDIRECT3DDEVICE9 GetDirect3DDevice();
	LPDIRECT3DSURFACE9 GetBackBuffer();
	LPD3DXSPRITE GetSpriteHandler();

	CTimer* GetTimer();
	CCamera* GetCamera();

	bool Load(string filePath);
	CScene* GetCurrentScene();
	string nextSceneId;
	void SwitchScene(string sceneId);

	static CGame* GetInstance();

	void AddScore(int addedScore);
	int GetScore();

	void AddHeart(int hearts);
	void DecreaseHeart(int hearts);
	int GetHearts();

	void AddLife(int lives);
	int GetLives();

	void TakeDamage(int volumes = 1);
	int getHealthVolumes();

	void SetSubWeaponType(string type);
	string GetSubWeaponType();

	void Reset();

	~CGame();
};


