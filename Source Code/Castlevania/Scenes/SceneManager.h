#pragma once
#include <string>
#include <vector>
#include <algorithm>

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
	string GetPreviousSceneId();

	void AddLoadedScenes(string sceneId);
	bool IsSceneLoaded(string sceneId);

protected:
	string currentSceneId;

	unordered_map<string, CScene*> scenes;
	vector<string> loadedScenes;

	static CSceneManager* instance;
};

