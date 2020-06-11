#include "WAxe.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Characters/Bosses/PhantomBat.h"
#include "../Misc/BigCandle.h"
#include "../Misc/SmallCandle.h"

WAxe::WAxe()
{
	SetAnimationSet("axe");

	elevation = WEAPON_DEFAULT_ELEVATION;
	vy = -AXE_MOVE_SPEED_Y;
	hasBoundingBox = true;
}

void WAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vx = directionX == Direction::Right ? AXE_MOVE_SPEED_X : -AXE_MOVE_SPEED_X;

	vy += AXE_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (y < CGame::GetInstance()->GetCamera()->GetTop() || y > CGame::GetInstance()->GetCamera()->GetBottom() || x > CGame::GetInstance()->GetCamera()->GetRight() || x < CGame::GetInstance()->GetCamera()->GetLeft())
		{
			CGame::GetInstance()->GetPlayerData()->DecreaseThrownSubWeapons();

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

			if (dynamic_cast<CEnemy*>(e->obj))
			{
				auto enemy = dynamic_cast<CEnemy*>(e->obj);

				enemy->TakeDamage(AXE_DAMAGES);

				if (dynamic_cast<CPhantomBat*>(e->obj))
				{
					hasBoundingBox = false;
				}

				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CBigCandle*>(e->obj))
			{
				auto candle = dynamic_cast<CBigCandle*>(e->obj);

				candle->Disappear();

				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CSmallCandle*>(e->obj))
			{
				auto candle = dynamic_cast<CSmallCandle*>(e->obj);

				candle->Disappear();

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

void WAxe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (hasBoundingBox)
	{
		left = x;
		top = y;
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
	}
}

void WAxe::Render()
{
	animationSet->at(AXE_ANI_MOVE)->Render(x, y);
}
