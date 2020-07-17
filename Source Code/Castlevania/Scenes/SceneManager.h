#pragma once
#include <string>
#include <vector>
#include <algorithm>

#include "../Libraries/TinyXML/tinyxml.h"
#include "Scene.h"
using namespace std;

class CSceneManager
{
public:
	static CSceneManager* GetInstance();

	bool Load(string filePath);
	bool IsSceneLoaded(string sceneId);

	CScene* GetCurrentScene();

	void Init(string filePath, string sharedResourcesPath);
	void ParseSounds(TiXmlElement* element);
	void ParseTextures(TiXmlElement* element);
	void ParseSprites(TiXmlElement* element);
	void ParseAnimations(TiXmlElement* element);
	void ParseAnimationSets(TiXmlElement* element);
	bool LoadSharedResources(string filePath);

	void SwitchScene(string sceneId, bool forced = false);
	void SwitchSceneByIndex(int index);
	void AddLoadedScenes(string sceneId);
	void ClearLoadedScenes();
	void Reset();

	string GetCurrentSceneId();
	string GetNextSceneId();
	string GetPreviousSceneId();
	string GetFirstSceneId();
	string GetSceneIdByIndex(int index);

	int GetIndexBySceneId(string sceneId);
	int GetCurrentSceneIndex();
	int GetNextSceneIndex();
	int GetPreviousSceneIndex();

protected:
	string currentSceneId;
	string sharedResourcesPath;

	unordered_map<string, CScene*> scenes;

	vector<string> loadedScenes;
	vector<string> sceneIds;

	static CSceneManager* instance;
};