#include "Scene.h"
#include "../Game.h"

CScene::CScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId, string requiredSceneId)
{
	this->id = id;
	this->filePath = filePath;
	this->previousSceneId = previousSceneId;
	this->nextSceneId = nextSceneId;
	this->stage = stage;
	this->switchSceneTime = -1;
	this->needReloading = false;
	this->requiredSceneId = requiredSceneId;
}

IKeyEventHandler* CScene::GetKeyEventHandler()
{
	return keyHandler;
}

void CScene::Pause()
{
	paused = true;
}

void CScene::Resume()
{
	paused = false;
}

bool CScene::Paused()
{
	return paused;
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

CGrid* CScene::GetGrid()
{
	return grid;
}
