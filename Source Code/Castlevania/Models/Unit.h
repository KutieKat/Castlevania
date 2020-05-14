#pragma once
#include "GameObject.h"
#include "Grid.h"

class CGrid;

class CUnit
{
public:
	CUnit(CGrid* grid, CGameObject* object, float x, float y);
	void Move(float x, float y);
	CGameObject* GetGameObject();

	float x;
	float y;

	CUnit* nextUnit;
	CUnit* previousUnit;

	CGameObject* object;
	CGrid* grid;

	void Clear();
};

