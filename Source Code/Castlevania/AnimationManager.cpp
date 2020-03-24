#include "AnimationManager.h"

CAnimationManager* CAnimationManager::instance = nullptr;

void CAnimationManager::Add(int id, CAnimation* animation)
{
	if (this->animations.find(id) == this->animations.end())
	{
		this->animations[id] = animation;
	}
}

CAnimation* CAnimationManager::Get(int id)
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
