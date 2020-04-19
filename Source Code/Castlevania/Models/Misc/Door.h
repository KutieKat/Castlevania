#pragma once
#include "../GameObject.h"
#include "DoorWall.h"
using namespace std;

class CDoor : public CGameObject
{
public:
	CDoor();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

