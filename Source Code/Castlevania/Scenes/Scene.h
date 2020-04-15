#pragma once
#include <string>
#include <d3dx9.h>

#include "../Input/IKeyEventHandler.h"
#include "../Models/Characters/Players/Simon.h"
using namespace std;

class CScene
{
protected:
	IKeyEventHandler* keyHandler;

	string id;
	string filePath;
	string nextSceneId;
	string stage;

	DWORD switchSceneTime;

public:
	CScene(string id, string filePath, string stage = "", string nextSceneId = "");

	IKeyEventHandler* GetKeyEventHandler();

	virtual bool Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;

	string GetId();
	string GetNextSceneId();
	string GetStage();
};

typedef CScene* LPSCENE;

class CSceneKeyHandler : public IKeyEventHandler
{
protected:
	CScene* scene;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int keyCode) = 0;
	virtual void OnKeyUp(int keyCode) = 0;
	CSceneKeyHandler(CScene* s) : IKeyEventHandler() { scene = s; }
};