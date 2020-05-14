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

	int GetIndexBySceneId(string sceneId)
	{
		for (int i = 0; i < sceneIds.size(); i++)
		{
			if (sceneIds[i] == sceneId)
			{
				return i;
			}
		}

		return 0;
	}

	int GetCurrentSceneIndex()
	{
		return GetIndexBySceneId(GetCurrentSceneId());
	}

	int GetNextSceneIndex()
	{
		return GetIndexBySceneId(GetNextSceneId());
	}

	int GetPreviousSceneIndex()
	{
		return GetIndexBySceneId(GetPreviousSceneId());
	}

	void AddLoadedScenes(string sceneId);
	void ClearLoadedScenes();

	bool IsSceneLoaded(string sceneId);

protected:
	string currentSceneId;

	unordered_map<string, CScene*> scenes;

	vector<string> loadedScenes;
	vector<string> sceneIds;

	static CSceneManager* instance;
};

