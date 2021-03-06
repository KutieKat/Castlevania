#include "SpriteManager.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Utilities/Debug.h"
#include "../Textures/TextureManager.h"
#include "../Utilities/SafeDelete.h"

CSpriteManager* CSpriteManager::instance = nullptr;

bool CSpriteManager::LoadFromFile(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Error(doc.ErrorDesc());
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

void CSpriteManager::Add(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture, bool sharable)
{
	CSprite* sprite = new CSprite(id, left, top, right, bottom, texture);

	if (sharable)
	{
		sharedSprites[id] = sprite;
	}
	else
	{
		sprites[id] = sprite;
	}
}

CSprite* CSpriteManager::Get(string id)
{
	bool foundInSprites = sprites[id] != nullptr;
	bool foundInSharedSprites = sharedSprites[id] != nullptr;

	if (foundInSprites || foundInSharedSprites)
	{
		if (foundInSprites)
		{
			return sprites[id];
		}
		else
		{
			return sharedSprites[id];
		}
	}
	else
	{
		CDebug::Error("Failed to find sprite id=" + id, "SpriteManager.cpp");
	}
}

CSprite* &CSpriteManager::operator[](string id)
{
	return sprites[id];
}

void CSpriteManager::Clear()
{
	for (auto x : sprites)
	{
		CSprite* sprite = x.second;
		SAFE_DELETE(sprite);
	}

	sprites.clear();
}

CSpriteManager* CSpriteManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CSpriteManager();
	}

	return instance;
}

CSpriteManager::~CSpriteManager()
{
	Clear();

	SAFE_DELETE(instance);
}
