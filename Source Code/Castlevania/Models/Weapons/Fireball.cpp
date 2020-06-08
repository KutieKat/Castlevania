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

	rotation = atan2(targetY - y, targetX - x);

	x += cos(rotation) * 0.1f * dt;
	y += sin(rotation) * 0.1f * dt;
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
	animationSet->at(0)->Render(x, y);
}

void WFireball::SetTarget(float x, float y)
{
	targetX = x;
	targetY = y;
}
