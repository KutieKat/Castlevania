#pragma once
#include <string>

#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"
using namespace std;

class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

public: 
	CMario() : CGameObject()
	{
		level = MARIO_LEVEL_BIG;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual CBoundingBox GetBoundingBox();
};