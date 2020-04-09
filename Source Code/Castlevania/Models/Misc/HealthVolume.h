#pragma once
#include <string>
#include <vector>

#include "../../Animations/Animation.h"
#include "../../Utilities/Constants.h"
using namespace std;

class CHealthVolume
{
public:
	void AddAnimation(string aniId);
	void Render();
	void SetPosition(float x, float y);
	void SetType(HealthType type);

private:
	HealthType type;

	float x;
	float y;

	vector<CAnimation*> animations;
};

