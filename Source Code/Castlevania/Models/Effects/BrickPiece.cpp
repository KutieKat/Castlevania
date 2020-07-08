#include "BrickPiece.h"
#include "../../Game.h"

CBrickPiece::CBrickPiece()
{
	isEffect = true;
	SetAnimationSet("brick_piece");
	elevation = EFFECT_DEFAULT_ELEVATION;
}

void CBrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	x += vx * dt;
	y += vy * dt;
}

void CBrickPiece::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CBrickPiece::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}
