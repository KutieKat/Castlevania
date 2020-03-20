#include "AnimationManager.h"

CAnimationManager* CAnimationManager::instance = nullptr;

void CAnimationManager::Add(string id, CAnimation* animation)
{
	animations[id] = animation;
}

CAnimation* CAnimationManager::Get(string id)
{
	return animations[id];
}

CAnimationManager* CAnimationManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CAnimationManager();
	}

	return instance;
}
