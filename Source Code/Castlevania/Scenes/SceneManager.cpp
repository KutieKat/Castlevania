#include "SceneManager.h"
#include "CutScene.h"
#include "IntroScene.h"
#include "PlayScene.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Utilities/Debug.h"

CSceneManager* CSceneManager::instance = nullptr;

CSceneManager* CSceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CSceneManager();
	}

	return instance;
}

bool CSceneManager::Load(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Info(doc.ErrorDesc(), "Game.cpp");
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* scene = nullptr;

	this->currentSceneId = root->Attribute("startSceneId");

	CScene* sceneItem = nullptr;

	for (scene = root->FirstChildElement(); scene != nullptr; scene = scene->NextSiblingElement())
	{
		bool needReloading = false;

		string id = scene->Attribute("id");
		string type = scene->Attribute("type");
		string filePath = scene->Attribute("path");
		string stage = scene->Attribute("stage");
		string previousSceneId = scene->Attribute("previousSceneId");
		string nextSceneId = scene->Attribute("nextSceneId");
		string requiredSceneId = scene->Attribute("requiredSceneId");

		scene->QueryBoolAttribute("needReloading", &needReloading);

		if (type == "play_scene")
		{
			sceneItem = new CPlayScene(id, filePath, stage, previousSceneId, nextSceneId, requiredSceneId);
		}

		if (type == "intro_scene")
		{
			sceneItem = new CIntroScene(id, filePath, stage, previousSceneId, nextSceneId, requiredSceneId);
		}

		if (type == "cut_scene")
		{
			sceneItem = new CCutScene(id, filePath, stage, previousSceneId, nextSceneId, requiredSceneId);
		}

		sceneItem->needReloading = needReloading;

		scenes[id] = sceneItem;
		sceneIds.emplace_back(id);
	}

	SwitchScene(currentSceneId);

	return true;
}

CScene* CSceneManager::GetCurrentScene()
{
	return scenes[currentSceneId];
}

void CSceneManager::SwitchScene(string sceneId, bool forced)
{
	CGame* game = CGame::GetInstance();

	scenes[currentSceneId]->Unload();

	CTextureManager::GetInstance()->Clear();
	CSpriteManager::GetInstance()->Clear();
	CAnimationManager::GetInstance()->Clear();
	CAnimationSets::GetInstance()->Clear();

	currentSceneId = sceneId;

	CScene* scene = scenes[sceneId];
	game->SetKeyHandler(scene->GetKeyEventHandler());
	game->GetCamera()->Reset();

	if (CGame::GetInstance()->GetTimer()->GetRemainingTime() == -1)
	{
		CGame::GetInstance()->GetTimer()->SetTime(DEFAULT_GAME_TIME);
	}

	if (dynamic_cast<CIntroScene*>(scene))
	{
		game->Reset();
	}

	if (dynamic_cast<CPlayScene*>(scene))
	{
		game->GetTimer()->Resume();
	}
	else
	{
		game->GetTimer()->Pause();
	}

	if (!forced && scene->needReloading && IsSceneLoaded(sceneId))
	{
		scene->Reload();
	}
	else
	{
		if (scene->GetPreviousSceneId() != "")
		{
			AddLoadedScenes(scene->GetPreviousSceneId());
		}

		scene->Load();
		AddLoadedScenes(sceneId);
	}
}

void CSceneManager::SwitchSceneByIndex(int index)
{
	if (CGame::GetInstance()->GetTimer()->GetRemainingTime() == -1)
	{
		CGame::GetInstance()->GetTimer()->SetTime(DEFAULT_GAME_TIME);
	}

	ClearLoadedScenes();

	CScene* scene = scenes[GetSceneIdByIndex(index)];
	string requiredSceneId = scene->GetRequiredSceneId();

	if (requiredSceneId != "")
	{
		AddLoadedScenes(requiredSceneId);
	}

	if (GetSceneIdByIndex(index) == requiredSceneId)
	{
		ClearLoadedScenes();
	}

	SwitchScene(GetSceneIdByIndex(index), true);
}

string CSceneManager::GetCurrentSceneId()
{
	return currentSceneId;
}

string CSceneManager::GetNextSceneId()
{
	return GetCurrentScene()->GetNextSceneId();
}

string CSceneManager::GetPreviousSceneId()
{
	return GetCurrentScene()->GetPreviousSceneId();
}

string CSceneManager::GetFirstSceneId()
{
	return GetSceneIdByIndex(0);
}

string CSceneManager::GetSceneIdByIndex(int index)
{
	int newIndex = index > scenes.size() ? 0 : index;

	return scenes[sceneIds[newIndex]]->GetId();
}

void CSceneManager::AddLoadedScenes(string sceneId)
{
	if (find(loadedScenes.begin(), loadedScenes.end(), sceneId) == loadedScenes.end())
	{
		loadedScenes.emplace_back(sceneId);
	}
}

void CSceneManager::ClearLoadedScenes()
{
	loadedScenes.clear();
}

void CSceneManager::Reset()
{
	ClearLoadedScenes();
}

bool CSceneManager::IsSceneLoaded(string sceneId)
{
	if (find(loadedScenes.begin(), loadedScenes.end(), sceneId) != loadedScenes.end())
	{
		return true;
	}

	return false;
}
