#include "Breaking.h"
#include "../../Game.h"
#include "../Grid.h"
#include "../Unit.h"
#include "../../Utilities/SafeDelete.h"

CBreaking::CBreaking(float x, float y)
{
	this->x = x;
	this->y = y;
	this->isEffect = true;

	for (int i = 0; i < BREAKING_PIECES; i++)
	{
		float speedX, speedY;
		GetPieceSpeed(i, speedX, speedY);

		CBrickPiece* piece = new CBrickPiece();
		piece->SetPosition(x, y);
		piece->SetVisibility(Visibility::Hidden);
		piece->SetSpeed(speedX, speedY);
		pieces.emplace_back(piece);

		CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
		CUnit* unit = new CUnit(grid, piece, x, y);
	}
}

void CBreaking::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (!removable)
	{
		for (int i = 0; i < pieces.size(); i++)
		{
			pieces[i]->SetVisibility(Visibility::Visible);
			pieces[i]->Update(dt);
		}
	}
	else
	{
		for (int i = 0; i < pieces.size(); i++)
		{
			pieces[i]->removable = true;
		}
	}
}

void CBreaking::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CBreaking::Render()
{
}

void CBreaking::GetPieceSpeed(int index, float & speedX, float & speedY)
{
	switch (index)
	{
	case 0:
		speedX = -0.05f;
		speedY = 0.05f;
		break;

	case 1:
		speedX = 0.1f;
		speedY = 0.1f;
		break;

	case 2:
		speedX = 0.1f;
		speedY = -0.1f;
		break;

	case 3:
		speedX = -0.05f;
		speedY = -0.05f;
		break;
	}
}
