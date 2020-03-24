#include "SpriteManager.h"
#include "tinyxml.h"
#include "debug.h"
#include "TextureManager.h"

CSpriteManager* CSpriteManager::instance = nullptr;

bool CSpriteManager::LoadFromFile(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		DebugOut(L"%s", doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* sprite = nullptr;

	CTextureManager* textures = CTextureManager::GetInstance();
	LPDIRECT3DTEXTURE9 texture = textures->Get(root->Attribute("textureId"));

	for (sprite = root->FirstChildElement(); sprite != nullptr; sprite = sprite->NextSiblingElement())
	{
		int left, top, right, bottom;

		string id = sprite->Attribute("id");

		sprite->QueryIntAttribute("left", &left);
		sprite->QueryIntAttribute("top", &top);
		sprite->QueryIntAttribute("right", &right);
		sprite->QueryIntAttribute("bottom", &bottom);

		Add(id, left, top, right, bottom, texture);
	}

	return true;
}

void CSpriteManager::Add(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture)
{
	if (this->sprites.find(id) == this->sprites.end())
	{
		CSprite* sprite = new CSprite(id, left, top, right, bottom, texture);
		this->sprites[id] = sprite;
	}
}

CSprite* CSpriteManager::Get(string id)
{
	return this->sprites[id];
}

CSprite* &CSpriteManager::operator[](string id)
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