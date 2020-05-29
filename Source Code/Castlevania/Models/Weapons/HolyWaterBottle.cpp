#include "HolyWaterBottle.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Misc/Brick.h"
#include "../Misc/Ground.h"
#include "../Misc/BottomStair.h"

CHolyWaterBottle::CHolyWaterBottle()
{
	SetAnimationSet("holy_water_bottle");
	vy = -HOLY_WATER_BOTTLE_MOVE_SPEED;

	SetState(HOLY_WATER_BOTTLE_STATE_MOVE);
}

void CHolyWaterBottle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case HOLY_WATER_BOTTLE_STATE_MOVE:
		break;

	case HOLY_WATER_BOTTLE_STATE_BURN:
		vy = 0;
		vx = 0;
		SetDisplayTime(HOLY_WATER_BOTTLE_BURNING_TIME);
		break;
	}
}

void CHolyWaterBottle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (state != HOLY_WATER_BOTTLE_STATE_BURN)
	{
		vx = directionX == Direction::Right ? HOLY_WATER_BOTTLE_MOVE_SPEED : -HOLY_WATER_BOTTLE_MOVE_SPEED;
		vy += HOLY_WATER_BOTTLE_GRAVITY * dt;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (x > CGame::GetInstance()->GetCamera()->GetRight() || x < CGame::GetInstance()->GetCamera()->GetLeft())
		{
			removable = true;
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

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CGround*>(e->obj))
			{
				if (e->nx != 0) vx = 0;
				if (e->ny != 0) vy = 0;

				SetState(HOLY_WATER_BOTTLE_STATE_BURN);
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				auto enemy = dynamic_cast<CEnemy*>(e->obj);

				if (state == HOLY_WATER_BOTTLE_STATE_BURN)
				{
					enemy->TakeDamage();
				}
				else
				{
					if (e->nx != 0) x += dx;
				}
			}
			else if (dynamic_cast<CBottomStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CHolyWaterBottle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + HOLY_WATER_BOTTLE_BBOX_WIDTH;
	bottom = top + HOLY_WATER_BOTTLE_BBOX_HEIGHT;
}

void CHolyWaterBottle::Render()
{
	int ani;

	switch (state)
	{
	case HOLY_WATER_BOTTLE_STATE_MOVE:
		ani = directionX == Direction::Right ? HOLY_WATER_BOTTLE_ANI_RIGHT : HOLY_WATER_BOTTLE_ANI_LEFT;
		break;

	case HOLY_WATER_BOTTLE_STATE_BURN:
		ani = HOLY_WATER_BOTTLE_ANI_FLAME;
		break;
	}
	
	animationSet->at(ani)->Render(x, y);
}
