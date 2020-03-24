#include "AnimationManager.h"
#include "tinyxml.h"
#include "debug.h"
#include "SpriteManager.h"

CAnimationManager* CAnimationManager::instance = nullptr;

bool CAnimationManager::LoadFromFile(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		DebugOut(L"%s", doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* animation = nullptr;

	CAnimation* ani = nullptr;

	for (animation = root->FirstChildElement(); animation != nullptr; animation = animation->NextSiblingElement())
	{
		int duration;

		string id = animation->Attribute("id");
		animation->QueryIntAttribute("duration", &duration);

		ani = new CAnimation(duration);

		TiXmlElement* frame = nullptr;

		for (frame = animation->FirstChildElement(); frame != nullptr; frame = frame->NextSiblingElement())
		{
			string spriteId = frame->Attribute("spriteId");
			ani->Add(spriteId);
		}

		Add(id, ani);
	}

	return true;
}

void CAnimationManager::Add(string id, CAnimation* animation)
{
	if (this->animations.find(id) == this->animations.end())
	{
		this->animations[id] = animation;
	}
}

CAnimation* CAnimationManager::Get(string id)
{
	return this->animations[id];
}

CAnimationManager* CAnimationManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CAnimationManager();
	}

	return instance;
}
