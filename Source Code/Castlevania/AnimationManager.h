#pragma once
#include <unordered_map>
#include "Animation.h"
using namespace std;

class CAnimationManager
{
public:
	void Add(int id, CAnimation* animation);
	CAnimation* Get(int id);

	static CAnimationManager* GetInstance();

private:
	static CAnimationManager* instance;

	unordered_map<int, CAnimation*> animations;
};

