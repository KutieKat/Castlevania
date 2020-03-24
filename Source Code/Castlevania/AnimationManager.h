#pragma once
#include <unordered_map>
#include <string>
#include "Animation.h"
using namespace std;

class CAnimationManager
{
public:
	bool LoadFromFile(string filePath);
	void Add(string id, CAnimation* animation);
	CAnimation* Get(string id);

	static CAnimationManager* GetInstance();

private:
	static CAnimationManager* instance;

	unordered_map<string, CAnimation*> animations;
};

