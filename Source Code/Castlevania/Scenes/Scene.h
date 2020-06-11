#pragma once
#include <string>
#include <d3dx9.h>

#include "../Input/IKeyEventHandler.h"
#include "../Models/Characters/Players/Simon.h"
#include "../Models/Grid.h"
using namespace std;

class CScene
{
protected:
	IKeyEventHandler* keyHandler;

	CGrid* grid;

	string id;
	string filePath;
	string previousSceneId;
	string nextSceneId;
	string stage;
	string requiredSceneId;

	DWORD switchSceneTime;
	DWORD softPauseTime;

	bool softPaused;
	bool hardPaused;
	bool showingPauseBadge;

public:
	CScene(string id, string filePath, string stage = "", string previousSceneId = "", string nextSceneId = "", string requiredSceneId = "");

	IKeyEventHandler* GetKeyEventHandler();

	virtual bool Load() = 0;
	virtual bool Reload() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;

	void SoftPause(DWORD duration = -1);
	void HardPause(bool showingLabel = true);

	void ResumeSoftPause();
	void ResumeHardPause();

	bool SoftPaused();
	bool HardPaused();
	bool ShowingPauseBadge();

	string GetId();
	string GetStage();
	string GetPreviousSceneId();
	string GetNextSceneId();
	string GetRequiredSceneId() { return requiredSceneId; }

	CGrid* GetGrid();

	bool needReloading;
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