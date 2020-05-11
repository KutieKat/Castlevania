#pragma once
#include "../GameObject.h"
using namespace std;

class CPreviousScene : public CGameObject
{
public:
	CPreviousScene();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();

	bool playerMustBeOnStair;
	Direction sceneDirection;
};

