#pragma once
#include <vector>
#include <d3d9.h>

#include "../Animations/Animation.h"
using namespace std;

class CEffect
{
public:
	CEffect(string type, DWORD timeout = 300);

	void SetAnimationSet(string animationSetId);
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

	CAnimationSet* animationSet;
};

