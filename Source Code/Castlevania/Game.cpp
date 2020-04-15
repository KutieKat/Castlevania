#include "Game.h"
#include "Utilities/Debug.h"
#include "Libraries/TinyXML/tinyxml.h"
#include "Scenes/PlayScene.h"
#include "Scenes/IntroScene.h"
#include "Scenes/CutScene.h"

CGame* CGame::instance = nullptr;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
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
	this->inputManager = CInputManager::GetInstance();
	this->inputManager->Init(hWnd);

	// Timer
	this->timer = new CTimer();

	// Camera
	this->camera = CCamera::GetInstance();
	this->camera->Init();

	// Player data
	score = 0;
	hearts = 5;
	lives = 3;
	healthVolumes = 16;
	subWeaponType.clear();

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

CTimer* CGame::GetTimer()
{
	return this->timer;
}

CCamera* CGame::GetCamera()
{
	return this->camera;
}

bool CGame::Load(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Info(doc.ErrorDesc(), "Game.cpp");
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* scene = nullptr;

	this->currentScene = root->Attribute("startSceneId");

	CScene* sceneItem = nullptr;

	for (scene = root->FirstChildElement(); scene != nullptr; scene = scene->NextSiblingElement())
	{
		string id = scene->Attribute("id");
		string type = scene->Attribute("type");
		string path = scene->Attribute("path");
		string stage = scene->Attribute("stage");
		string nextSceneId = scene->Attribute("nextSceneId");

		if (type == "play_scene")
		{
			sceneItem = new CPlayScene(id, path, stage, nextSceneId);
		}
		
		if (type == "intro_scene")
		{
			sceneItem = new CIntroScene(id, path, stage,nextSceneId);
		}

		if (type == "cut_scene")
		{
			sceneItem = new CCutScene(id, path, stage, nextSceneId);
		}

		scenes[id] = sceneItem;
	}

	SwitchScene(currentScene);

	return true;
}

LPSCENE CGame::GetCurrentScene()
{
	return scenes[currentScene];
}

void CGame::SwitchScene(string sceneId)
{
	// Unload TextureManager :(
	// Unload SpriteManager :(
	// Unload AnimationManager :(

	scenes[currentScene]->Unload();

	//CTextureManager::GetInstance()->Clear();
	//CSpriteManager::GetInstance()->Clear();
	//CAnimationManager::GetInstance()->Clear();

	currentScene = sceneId;
	CScene* scene = scenes[sceneId];
	CGame::GetInstance()->SetKeyHandler(scene->GetKeyEventHandler());

	if (dynamic_cast<CIntroScene*>(scene))
	{
		Reset();
	}
	
	scene->Load();
	CGame::GetInstance()->GetTimer()->SetTime(300);
}

CGame* CGame::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CGame();
	}

	return instance;
}

void CGame::AddScore(int addedScore)
{
	this->score += addedScore;
}

int CGame::GetScore()
{
	return this->score;
}

void CGame::AddHeart(int hearts)
{
	this->hearts += hearts;
}

void CGame::DecreaseHeart(int hearts)
{
	this->hearts -= hearts;
}

int CGame::GetHearts()
{
	return this->hearts;
}

void CGame::AddLife(int lives)
{
	this->lives += lives;
}

int CGame::GetLives()
{
	return this->lives;
}

void CGame::TakeDamage(int volumes)
{
	this->healthVolumes -= volumes;
}

int CGame::getHealthVolumes()
{
	return this->healthVolumes;
}

void CGame::SetSubWeaponType(string type)
{
	this->subWeaponType = type;
}

string CGame::GetSubWeaponType()
{
	return this->subWeaponType;
}

void CGame::Reset()
{
	score = 0;
	hearts = 5;
	lives = 3;
	healthVolumes = 16;
	subWeaponType.clear();
	camera->SetPosition(0, 0);
}
