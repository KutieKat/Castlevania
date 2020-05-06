#include "WBoomerang.h"
#include "../../Utilities/Debug.h"

WBoomerang::WBoomerang(CSimon* simon)
{
	SetAnimationSet("boomerang");

	this->simon = simon;
	this->direction = simon->direction;
	this->vx = simon->direction == Direction::Right ? BOOMERANG_MOVE_SPEED : -BOOMERANG_MOVE_SPEED;
	this->maxRight = simon->x + 200;
	this->maxLeft = simon->x - 200;
	this->hidden = false;
}

void WBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	x += vx * dt;

	if (x <= maxLeft || x >= maxRight)
	{
		vx = -vx;
	}

	if ((direction == Direction::Right && x < simon->x + SIMON_BBOX_WIDTH) || (direction == Direction::Left && x > simon->x))
	{
		hidden = true;
		vx = 0;
	}
}

void WBoomerang::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!hidden)
	{
		l = x;
		t = y;
		r = l + BOOMERANG_BBOX_WIDTH;
		b = t + BOOMERANG_BBOX_HEIGHT;
	}
}

void WBoomerang::Render()
{
	if (!hidden)
	{
		animationSet->at(BOOMERANG_ANI_MOVE)->Render(x, y);
	}
}
