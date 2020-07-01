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

	void SwitchScene(string sceneId, bool forced = false);
	void SwitchSceneByIndex(int index);

	string GetCurrentSceneId();
	string GetNextSceneId();
	string GetPreviousSceneId();
	string GetFirstSceneId();
	string GetSceneIdByIndex(int index);

	int GetIndexBySceneId(string sceneId);
	int GetCurrentSceneIndex();
	int GetNextSceneIndex();
	int GetPreviousSceneIndex();

	void AddLoadedScenes(string sceneId);
	void ClearLoadedScenes();

	void Reset();

	bool IsSceneLoaded(string sceneId);

protected:
	string currentSceneId;

	unordered_map<string, CScene*> scenes;

	vector<string> loadedScenes;
	vector<string> sceneIds;

	static CSceneManager* instance;
};

