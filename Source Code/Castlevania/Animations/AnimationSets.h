#pragma once
#include <unordered_map>

#include "Animation.h"
using namespace std;

class CAnimationSets
{
public:
	static CAnimationSets* GetInstance();

	void Add(string id, CAnimationSet* animationSet);
	CAnimationSet* Get(string id);
	void Clear();

	~CAnimationSets();

protected:
	static CAnimationSets* instance;

	unordered_map<string, CAnimationSet*> animationSets;
};

