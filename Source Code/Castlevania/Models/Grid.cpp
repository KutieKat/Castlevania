#include "Grid.h"
#include "../Utilities/SafeDelete.h"

CGrid::CGrid(int width, int height, int cellWidth, int cellHeight)
{
	this->width = width;
	this->height = height;
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;
	this->columns = width / cellWidth;
	this->rows = height / cellHeight;

	InitCells();
}

void CGrid::InitCells()
{
	cells.resize(rows);

	for (int row = 0; row < rows; row++)
	{
		cells[row].resize(columns);
	}

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			cells[row][column] = nullptr;
		}
	}
}

void CGrid::Add(CUnit* unit)
{
	int column = (int)(unit->x / cellWidth);
	int row = (int)(unit->y / cellHeight);

	unit->nextUnit = cells[row][column];
	unit->previousUnit = nullptr;
	cells[row][column] = unit;

	if (unit->nextUnit != nullptr)
	{
		unit->nextUnit->previousUnit = unit;
	}
}

void CGrid::Move(CUnit* unit, float x, float y)
{
	int oldRow = (int)(unit->y / cellHeight);
	int oldColumn = (int)(unit->x / cellWidth);

	int newRow = (int)(y / cellHeight);
	int newColumn = (int)(x / cellWidth);

	unit->x = x;
	unit->y = y;

	if (!InLimitedArea(newRow, newColumn) || SameCell(oldRow, oldColumn, newRow, newColumn))
	{
		return;
	}

	if (unit->nextUnit != nullptr)
	{
		unit->nextUnit->previousUnit = unit->previousUnit;
	}

	if (unit->previousUnit != nullptr)
	{
		unit->previousUnit->nextUnit = unit->nextUnit;
	}

	if (cells[oldRow][oldColumn] == unit)
	{
		cells[oldRow][oldColumn] = unit->nextUnit;
	}

	Add(unit);
}

void CGrid::Clean()
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			auto unit = cells[row][column];

			while (unit != nullptr)
			{
				if (unit->GetGameObject() && unit->GetGameObject()->removable)
				{
					unit->Clear();
				}

				unit = unit->nextUnit;
			}
		}
	}
}

vector<CUnit*> CGrid::Get(CCamera* camera)
{
	vector<CUnit*> units;

	int startingColumn = camera->GetLeft() / cellWidth;
	int endingColumn = ceil(camera->GetRight() / cellWidth);

	for (int row = 0; row < rows; row++)
	{
		for (int column = startingColumn; column < endingColumn; column++)
		{
			auto unit = cells[row][column];

			while (unit != nullptr)
			{
				if (unit->GetGameObject() && !unit->GetGameObject()->removable && unit->GetGameObject()->GetVisibility() == Visibility::Visible)
				{
					units.emplace_back(unit);
				}

				unit = unit->nextUnit;
			}
		}
	}

	return units;
}

bool CGrid::InLimitedArea(int row, int column)
{
	return !(column >= columns || column < 0 || row >= rows || row < 0);
}

bool CGrid::SameCell(int oldColumn, int oldRow, int newColumn, int newRow)
{
	return newColumn == oldColumn && newRow == oldRow;
}

CGrid::~CGrid()
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			auto cell = cells[row][column];

			SAFE_DELETE(cell);
		}

		cells[row].clear();
	}

	cells.clear();
}
