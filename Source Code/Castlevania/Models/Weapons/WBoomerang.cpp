#include "WBoomerang.h"
#include "../../Utilities/Debug.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Misc/BigCandle.h"
#include "../Misc/SmallCandle.h"
#include "../../Game.h"

WBoomerang::WBoomerang(CSimon* simon)
{
	SetAnimationSet("boomerang");

	this->simon = simon;
	this->elevation = 2;
	this->collisionCount = 0;
	this->directionX = simon->directionX;
	this->vx = simon->directionX == Direction::Right ? BOOMERANG_MOVE_SPEED : -BOOMERANG_MOVE_SPEED;
	this->maxRight = simon->x + BOOMERANG_MOVABLE_AREA_WIDTH;
	this->maxLeft = simon->x - BOOMERANG_MOVABLE_AREA_WIDTH;
}

void WBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if ((directionX == Direction::Right && x < simon->x + SIMON_BBOX_WIDTH && y > simon->y) || (directionX == Direction::Left && x > simon->x && y > simon->y))
	{
		CGame::GetInstance()->GetPlayerData()->DecreaseThrownSubWeapons();

		removable = true;
	}

	if (collisionCount == 2)
	{
		CGame::GetInstance()->GetPlayerData()->DecreaseThrownSubWeapons();

		removable = true;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (x <= maxLeft || x <= CGame::GetInstance()->GetCamera()->GetLeft() + BOOMERANG_BBOX_WIDTH - 20 || x >= maxRight || x >= CGame::GetInstance()->GetCamera()->GetRight() - BOOMERANG_BBOX_WIDTH - 20)
		{
			vx = -vx;
			collisionCount++;
		}

		if (x > CGame::GetInstance()->GetCamera()->GetRight() || x < CGame::GetInstance()->GetCamera()->GetLeft())
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

				enemy->TakeDamage(BOOMERANG_DAMAGES);

				if (e->nx != 0) x += dx;
			}
			else if (dynamic_cast<CBigCandle*>(e->obj))
			{
				auto candle = dynamic_cast<CBigCandle*>(e->obj);

				candle->Disappear();

				if (e->nx != 0) x += dx;
			}
			else if (dynamic_cast<CSmallCandle*>(e->obj))
			{
				auto candle = dynamic_cast<CSmallCandle*>(e->obj);

				candle->Disappear();

				if (e->nx != 0) x += dx;
			}
			else
			{
				if (e->nx != 0) x += dx;
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
	l = x;
	t = y;
	r = l + BOOMERANG_BBOX_WIDTH;
	b = t + BOOMERANG_BBOX_HEIGHT;
}

void WBoomerang::Render()
{
	animationSet->at(BOOMERANG_ANI_MOVE)->Render(x, y);
}
