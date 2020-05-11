#include "WBoomerang.h"
#include "../../Utilities/Debug.h"

WBoomerang::WBoomerang(CSimon* simon)
{
	SetAnimationSet("boomerang");

	this->simon = simon;
	this->directionX = simon->directionX;
	this->vx = simon->directionX == Direction::Right ? BOOMERANG_MOVE_SPEED : -BOOMERANG_MOVE_SPEED;
	this->maxRight = simon->x + 200;
	this->maxLeft = simon->x - 200;
	this->hidden = false;
}

void WBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if ((directionX == Direction::Right && x < simon->x + SIMON_BBOX_WIDTH) || (directionX == Direction::Left && x > simon->x))
	{
		hidden = true;
		vx = 0;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (x <= maxLeft || x >= maxRight)
		{
			vx = -vx;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->isEnemy)
			{
				e->obj->Disappear();
			}
			else
			{
				vx = -vx;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
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
