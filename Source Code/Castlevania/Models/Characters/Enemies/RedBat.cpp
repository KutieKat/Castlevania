#include "RedBat.h"

CRedBat::CRedBat()
{
	isEnemy = true;
	SetAnimationSet("red_bat");

	direction = Direction::Right;

	SetState(RED_BAT_STATE_STATIC);
}

void CRedBat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case RED_BAT_STATE_STATIC:
		vx = 0;
		break;

	case RED_BAT_STATE_MOVE:
		vx = direction == Direction::Right ? RED_BAT_MOVE_SPEED : -RED_BAT_MOVE_SPEED;
		break;
	}
}

void CRedBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (state == RED_BAT_STATE_MOVE)
	{
		x += vx * dt;
		y += sin(x * PI / 180);
	}
}

void CRedBat::Render()
{
	int ani;

	switch (state)
	{
	case RED_BAT_STATE_STATIC:
		ani = direction == Direction::Right ? RED_BAT_ANI_STATIC_RIGHT : RED_BAT_ANI_STATIC_LEFT;
		break;

	case RED_BAT_STATE_MOVE:
		ani = direction == Direction::Right ? RED_BAT_ANI_MOVE_RIGHT : RED_BAT_ANI_MOVE_LEFT;
		break;
	}

	if (!showingEffect)
	{
		animationSet->at(ani)->Render(x, y);
	}
}

void CRedBat::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEffect)
	{
		l = x;
		t = y;
		r = l + RED_BAT_BBOX_WIDTH;
		b = t + RED_BAT_BBOX_HEIGHT;
	}
}

bool CRedBat::IsPlayerNearby(float playerX, float playerY, float radius)
{
	if (pow(playerX - x, 2) + pow(playerY - y, 2) <= pow(radius, 2))
	{
		return true;
	}

	return false;
}