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
			sceneItem = new CIntroScene(id, path, stage, nextSceneId);
		}

		if (type == "cut_scene")
		{
			sceneItem = new CCutScene(id, path, stage, nextSceneId);
		}

		scenes[id] = sceneItem;
	}

	SwitchScene(currentSceneId);

	return true;
}

CScene* CSceneManager::GetCurrentScene()
{
	return scenes[currentSceneId];
}

void CSceneManager::SwitchScene(string sceneId)
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

	if (dynamic_cast<CIntroScene*>(scene))
	{
		game->Reset();
	}

	scene->Load();
	game->GetTimer()->SetTime(300);
}

string CSceneManager::GetCurrentSceneId()
{
	return currentSceneId;
}

string CSceneManager::GetNextSceneId()
{
	return GetCurrentScene()->GetNextSceneId();
}
