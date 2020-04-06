#pragma once
#include <vector>
#include <d3d9.h>

#include "../Animations/Animation.h"
using namespace std;

class CEffect
{
public:
	void AddAnimation(string aniId);
	void SetPosition(float x, float y);
	void GetPosition(float &x, float &y);
	void SetTimeout(DWORD timeout);
	void SetStartTime(DWORD startTime);
	void Render();

	DWORD GetStartTime();

	bool Over();

protected:
	DWORD timeout;
	DWORD startTime = -1;

	float x;
	float y;

	vector<CAnimation*> animations;
};

