#include "AnimationManager.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Utilities/Debug.h"
#include "../Sprites/SpriteManager.h"
#include "../Utilities/SafeDelete.h"

CAnimationManager* CAnimationManager::instance = nullptr;

bool CAnimationManager::LoadFromFile(string filePath)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Info(doc.ErrorDesc(), "AnimationManager.cpp");
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* animation = nullptr;
	TiXmlElement* frame = nullptr;

	CAnimation* ani = nullptr;

	for (animation = root->FirstChildElement(); animation != nullptr; animation = animation->NextSiblingElement())
	{
		string id = animation->Attribute("id");

		ani = new CAnimation(100);

		for (frame = animation->FirstChildElement(); frame != nullptr; frame = frame->NextSiblingElement())
		{
			int duration;

			string spriteId = frame->Attribute("spriteId");
			frame->QueryIntAttribute("duration", &duration);

			ani->Add(spriteId, duration);
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

void CAnimationManager::Clear()
{
	for (auto x : animations)
	{
		CAnimation* ani = x.second;
		SAFE_DELETE(ani);
	}

	animations.clear();
}

CAnimation* CAnimationManager::Get(string id)
{
	CAnimation* ani = this->animations[id];

	if (ani == nullptr)
	{
		CDebug::Error("Failed to find animation id=" + id, "AnimationManager.cpp");
	}

	return ani;
}

CAnimationManager* CAnimationManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CAnimationManager();
	}

	return instance;
}

CAnimationManager::~CAnimationManager()
{
	Clear();

	SAFE_DELETE(instance);
}