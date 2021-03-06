﻿#include "AnimationManager.h"
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

void CAnimationManager::Add(string id, CAnimation* animation, bool sharable)
{
	if (sharable)
	{
		sharedAnimations[id] = animation;
	}
	else
	{
		animations[id] = animation;
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
	bool foundInAnimations = animations[id] != nullptr;
	bool foundInSharedAnimations = sharedAnimations[id] != nullptr;

	if (foundInAnimations || foundInSharedAnimations)
	{
		if (foundInAnimations)
		{
			return animations[id];
		}
		else
		{
			return sharedAnimations[id];
		}
	}
	else
	{
		CDebug::Error("Failed to find animation id=" + id, "AnimationManager.cpp");
	}
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