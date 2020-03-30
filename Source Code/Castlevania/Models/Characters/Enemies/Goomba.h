#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual CBoundingBox GetBoundingBox();

public: 	
	virtual void SetState(int state);
};