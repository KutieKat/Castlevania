#pragma once
#include "../GameObject.h"
#include "DoorWall.h"

class CDoor : public CGameObject
{
public:
	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();

	void SetDoorWall(CDoorWall* doorWall);
	CDoorWall* GetDoorWall();

protected:
	CDoorWall* doorWall;
};

