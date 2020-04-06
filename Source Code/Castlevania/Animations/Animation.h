#pragma once
#include <d3d9.h>
#include <vector>
#include <string>
#include "AnimationFrame.h"
using namespace std;

class CAnimation
{
public:
	CAnimation(int defaultTime);

	void Add(string spriteId, DWORD time = 0);
	void Render(float x, float y, int alpha = 255);
	void SetStartTime(DWORD start);
	void Reset();

	int GetCurrentFrame();
	int GetTotalFrames();

	bool Over();

private:
	DWORD lastFrameTime;
	DWORD startTime;

	int defaultTime;
	int currentFrame;

	vector<CAnimationFrame*> frames;
};

