#include "Unit.h"
#include "../Utilities/SafeDelete.h"

CUnit::CUnit(CGrid* grid, CGameObject* object, float x, float y)
{
	this->grid = grid;
	this->object = object;

	this->x = x;
	this->y = y;

	this->nextUnit = nullptr;
	this->previousUnit = nullptr;

	if (grid)
	{
		grid->Add(this);
	}
}

void CUnit::Move(float x, float y)
{
	if (grid)
	{
		grid->Move(this, x, y);
	}
}

CGameObject* CUnit::GetGameObject()
{
	return object;
}

void CUnit::Clear()
{
	SAFE_DELETE(object);
}
