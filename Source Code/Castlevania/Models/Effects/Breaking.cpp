#include "Breaking.h"

CBreaking::CBreaking(float x, float y)
{
	this->x = x;
	this->y = y;
	this->isEffect = true;

	for (int i = 0; i < 4; i++)
	{
		CBrickPiece* piece = new CBrickPiece();
		piece->SetPosition(x, y);
		piece->SetVisibility(Visibility::Hidden);

		this->pieces.emplace_back(piece);
	}
}

void CBreaking::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	for (int i = 0; i < pieces.size(); i++)
	{
		float pieceVx, pieceVy;

		switch (i)
		{
		case 0:
			pieceVx = -0.1f;
			pieceVy = 0.1f;
			break;

		case 1:
			pieceVx = 0.2f;
			pieceVy = 0.2f;
			break;

		case 2:
			pieceVx = 0.2f;
			pieceVy = -0.2f;
			break;

		case 3:
			pieceVx = -0.1f;
			pieceVy = -0.1f;
			break;
		}

		pieces[i]->SetSpeed(pieceVx, pieceVy);
		pieces[i]->SetVisibility(Visibility::Visible);
		pieces[i]->Update(dt);
	}
}

void CBreaking::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CBreaking::Render()
{
	for (int i = 0; i < pieces.size(); i++)
	{
		pieces[i]->Render();
	}
}
