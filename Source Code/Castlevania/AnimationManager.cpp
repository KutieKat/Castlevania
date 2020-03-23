#include "AnimationManager.h"

CAnimationManager* CAnimationManager::instance = nullptr;

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
