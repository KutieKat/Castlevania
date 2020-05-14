#pragma once
#include "GameObject.h"
#include "../Camera/Camera.h"
#include "Unit.h"

class CUnit;

class CGrid
{
public:
	CGrid(int width, int height, int cellWidth, int cellHeight);
	void Add(CUnit* unit);
	void Move(CUnit* unit, float x, float y);
	void Clean();

	vector<CUnit*> Get(CCamera* camera);

	~CGrid();

protected:
	vector<vector<CUnit*>> cells;

	int width;
	int height;

	int cellWidth;
	int cellHeight;

	int columns;
	int rows;

	bool InLimitedArea(int row, int column);
	bool SameCell(int oldRow, int oldColumn, int newRow, int newColumn);

	void InitCells();
};

