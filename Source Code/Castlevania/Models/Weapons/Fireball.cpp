#include "Fireball.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"

WFireball::WFireball()
{
	SetAnimationSet("fireball");

	elevation = WEAPON_DEFAULT_ELEVATION;
}

void WFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (y < targetY)
	{
		angle = atan2(targetY - y, targetX - x);

		x += cos(angle) * FIREBALL_MOVE_SPEED_X * dt;
		y += sin(angle) * FIREBALL_MOVE_SPEED_Y * dt;
	}
	else
	{
		x += cos(angle) * FIREBALL_MOVE_SPEED_X * dt;
		y += FIREBALL_MOVE_SPEED_Y * dt;
	}
}

void WFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

void WFireball::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void WFireball::SetTarget(float x, float y)
{
	targetX = x;
	targetY = y;
}