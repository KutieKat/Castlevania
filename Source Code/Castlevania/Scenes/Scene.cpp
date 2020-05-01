#include "Scene.h"

CScene::CScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId)
{
	this->id = id;
	this->filePath = filePath;
	this->previousSceneId = previousSceneId;
	this->nextSceneId = nextSceneId;
	this->stage = stage;
	this->switchSceneTime = -1;
	this->needReloading = false;
}

IKeyEventHandler* CScene::GetKeyEventHandler()
{
	return keyHandler;
}

string CScene::GetId()
{
	return id;
}

string CScene::GetPreviousSceneId()
{
	return previousSceneId;
}

string CScene::GetNextSceneId()
{
	return nextSceneId;
}

string CScene::GetStage()
{
	return stage;
}
