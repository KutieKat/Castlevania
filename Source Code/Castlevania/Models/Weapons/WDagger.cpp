#include "WDagger.h"
#include "../../Game.h"

WDagger::WDagger()
{
	SetAnimationSet("dagger");
	this->vx = directionX == Direction::Right ? DAGGER_MOVE_SPEED : -DAGGER_MOVE_SPEED;
}

void WDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (x > CGame::GetInstance()->GetCamera()->GetRight())
		{
			SetVisibility(Visibility::Hidden);
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

void WDagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

void WDagger::Render()
{
	int ani = directionX == Direction::Right ? DAGGER_ANI_RIGHT : DAGGER_ANI_LEFT;

	animationSet->at(ani)->Render(x, y);
}
