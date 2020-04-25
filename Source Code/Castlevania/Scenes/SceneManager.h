#pragma once
#include <string>

#include "Scene.h"
using namespace std;

class CSceneManager
{
public:
	static CSceneManager* GetInstance();

	bool Load(string filePath);
	CScene* GetCurrentScene();
	void SwitchScene(string sceneId);

	string GetCurrentSceneId();
	string GetNextSceneId();

protected:
	string currentSceneId;

	unordered_map<string, CScene*> scenes;

	static CSceneManager* instance;
};

