#pragma once
#include "../GameObject.h"

class CWeapon : public CGameObject
{
public:
	virtual void Upgrade();
	int GetLevel();

protected:
	int level;
};

