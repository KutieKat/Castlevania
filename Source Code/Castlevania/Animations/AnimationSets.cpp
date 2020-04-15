#include "AnimationSets.h"
#include "../Utilities/Debug.h"
#include "../Utilities/SafeDelete.h"

CAnimationSets* CAnimationSets::instance = nullptr;

void CAnimationSets::Add(string id, CAnimationSet* animationSet)
{
	animationSets[id] = animationSet;
}

CAnimationSet* CAnimationSets::Get(string id)
{
	CAnimationSet* animationSet = animationSets[id];

	if (animationSet == nullptr)
	{
		CDebug::Error("Failed to find animation set id=" + id, "AnimationSets.cpp");
	}

	return animationSet;
}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CAnimationSets();
	}

	return instance;
}

CAnimationSets::~CAnimationSets()
{
	for (auto x : animationSets)
	{
		CAnimationSet* animationSet = x.second;
		SAFE_DELETE(animationSet);
	}

	animationSets.clear();

	SAFE_DELETE(instance);
}