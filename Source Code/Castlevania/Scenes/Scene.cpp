#include "Scene.h"

CScene::CScene(string id, string filePath, string stage, string nextSceneId)
{
	this->id = id;
	this->filePath = filePath;
	this->nextSceneId = nextSceneId;
	this->stage = stage;
	this->switchSceneTime = -1;
}

IKeyEventHandler* CScene::GetKeyEventHandler()
{
	return this->keyHandler;
}

string CScene::GetId()
{
	return this->id;
}

string CScene::GetNextSceneId()
{
	return this->nextSceneId;
}

string CScene::GetStage()
{
	return this->stage;
}
