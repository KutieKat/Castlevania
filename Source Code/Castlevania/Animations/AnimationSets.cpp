#include "AnimationSets.h"
#include "../Utilities/Debug.h"
#include "../Utilities/SafeDelete.h"

CAnimationSets* CAnimationSets::instance = nullptr;

void CAnimationSets::Add(string id, CAnimationSet* animationSet, bool sharable)
{
	if (sharable)
	{
		sharedAnimationSets[id] = animationSet;
	}
	else
	{
		animationSets[id] = animationSet;
	}
}

CAnimationSet* CAnimationSets::Get(string id)
{
	bool foundInAnimationSets = animationSets[id] != nullptr;
	bool foundInSharedAnimationSets = sharedAnimationSets[id] != nullptr;

	if (foundInAnimationSets || foundInSharedAnimationSets)
	{
		if (foundInAnimationSets)
		{
			return animationSets[id];
		}
		else
		{
			return sharedAnimationSets[id];
		}
	}
	else
	{
		CDebug::Error("Failed to find animation set id=" + id, "AnimationSets.cpp");
	}
}

void CAnimationSets::Clear()
{
	for (auto x : animationSets)
	{
		CAnimationSet* animationSet = x.second;
		SAFE_DELETE(animationSet);
	}

	animationSets.clear();
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
	Clear();

	SAFE_DELETE(instance);
}