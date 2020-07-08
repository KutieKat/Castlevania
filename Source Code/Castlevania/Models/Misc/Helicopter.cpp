#include "Helicopter.h"
#include "../../Game.h"

CHelicopter::CHelicopter()
{
	SetAnimationSet("helicopter");
}

void CHelicopter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vx = vy = HELICOPTER_FLY_SPEED;

	x -= vx * dt;
}

void CHelicopter::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CHelicopter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}