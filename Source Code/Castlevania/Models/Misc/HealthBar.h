#pragma once
#include "HealthVolume.h"

class CHealthBar
{
public:
	CHealthBar(HealthType type, int max, int value);

	void SetPosition(float x, float y);
	void SetValue(int value);
	void Render();

protected:
	vector<CHealthVolume*> volumes;

	HealthType type;

	float x;
	float y;

	int max;
	int value;

	void Update();
};

