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

public:
	CScene(std::string id, std::string filePath);

	IKeyEventHandler* GetKeyEventHandler();

	virtual bool Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual CSimon* GetPlayer() = 0;
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