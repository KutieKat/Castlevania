#pragma once
#include "../GameObject.h"
#include "../../Effects/Effect.h"

class CItem : public CGameObject
{
public:
	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom) = 0;
	virtual CBoundingBox GetBoundingBox() = 0;
	virtual void Render() = 0;

	//virtual void Appear();
	//virtual void Disappear();

	//void SetStartingEffect(CEffect* effect);
	//void SetEndingEffect(CEffect* effect);

//protected:
//	CEffect* startEffect;
//	CEffect* endEffect;
//
//	bool started = false;
//	bool ended = false;
};

