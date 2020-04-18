#pragma once
#include <vector>
#include "HealthVolume.h"
using namespace std;

class CHealthBar
{
public:
	CHealthBar(HealthType type, int max, int value);

	void SetPosition(float x, float y);
	void SetValue(int value);
	void Render();
	void Update();

protected:
	vector<CHealthVolume*> volumes;

	HealthType type;

	float x;
	float y;

	int max;
	int value;
};

