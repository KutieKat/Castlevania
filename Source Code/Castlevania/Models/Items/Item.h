#pragma once
#include "../GameObject.h"
#include "../../Effects/Effect.h"

class CItem : public CGameObject
{
public:
	virtual void Disappear();
	void SetDisplayTime(DWORD time);

protected:
	DWORD disappearingTime = -1;
};

