#pragma once
#include "../GameObject.h"

class CItem : public CGameObject
{
public:
	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom) = 0;
	virtual CBoundingBox GetBoundingBox() = 0;
	virtual void Render() = 0;
};

