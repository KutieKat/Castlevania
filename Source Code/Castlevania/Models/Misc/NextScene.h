#pragma once
#include "../GameObject.h"
using namespace std;

class CNextScene : public CGameObject
{
public:
	CNextScene();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();

	bool playerMustBeOnStair;
};

