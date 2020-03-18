#include "SpriteManager.h"

CSpriteManager* CSpriteManager::instance = nullptr;

void CSpriteManager::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture)
{
	CSprite* s = new CSprite(id, left, top, right, bottom, texture);
	sprites[id] = s;
}

CSprite* CSpriteManager::Get(int id)
{
	return sprites[id];
}

CSprite* &CSpriteManager::operator[](int id)
{
	return sprites[id];
}

CSpriteManager* CSpriteManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CSpriteManager();
	}

	return instance;
}