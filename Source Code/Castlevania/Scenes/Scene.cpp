#include "Scene.h"

CScene::CScene(string id, string filePath)
{
	this->id = id;
	this->filePath = filePath;
}

IKeyEventHandler* CScene::GetKeyEventHandler()
{
	return this->keyHandler;
}
