#include "SpriteManager.h"

CSpriteManager* CSpriteManager::instance = nullptr;

void CSpriteManager::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture)
{
	if (this->sprites.find(id) == this->sprites.end())
	{
		CSprite* sprite = new CSprite(id, left, top, right, bottom, texture);
		this->sprites[id] = sprite;
	}
}

CSprite* CSpriteManager::Get(int id)
{
	return this->sprites[id];
}

CSprite* &CSpriteManager::operator[](int id)
{
	return this->sprites[id];
}

CSpriteManager* CSpriteManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CSpriteManager();
	}

	return instance;
}