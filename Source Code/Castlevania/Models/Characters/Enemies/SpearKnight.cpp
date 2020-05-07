#include "SpearKnight.h"
#include "../../../Game.h"
#include "../../Misc/BottomStair.h"

CSpearKnight::CSpearKnight()
{
	isEnemy = true;
	SetAnimationSet("spear_knight");

	direction = Direction::Right;
	remainingHits = 2;
	delayTimeout = -1;
	SetState(SPEAR_KNIGHT_STATE_WALK);
}

void CSpearKnight::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SPEAR_KNIGHT_STATE_WALK:
		vx = direction == Direction::Right ? SPEAR_KNIGHT_WALK_SPEED : -SPEAR_KNIGHT_WALK_SPEED;
		break;

	case SPEAR_KNIGHT_STATE_DELAY:
		vx = 0;
		delayTimeout = GetTickCount() + 300;
		break;
	}
}

void CSpearKnight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (x <= leftBound)
	{
		direction = Direction::Right;
		vx = -vx;
	}
	else if (x >= rightBound)
	{
		direction = Direction::Left;
		vx = -vx;
	}

	dx = vx * dt;
	vy += SPEAR_KNIGHT_GRAVITY * dt;

	if (remainingHits <= 0)
	{
		CGame::GetInstance()->GetPlayerData()->AddScore(400);

		Disappear();
		remainingHits = 2;
	}

	if (delayTimeout != -1 && GetTickCount() > delayTimeout)
	{
		SetState(SPEAR_KNIGHT_STATE_WALK);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
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

			if (dynamic_cast<CBottomStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
			}
			else
			{
				if (e->ny != 0) vy = 0;
			}
		}
	}
}

void CSpearKnight::Render()
{
	int ani;

	switch (state)
	{
	case SPEAR_KNIGHT_STATE_DELAY:
		ani = direction == Direction::Right ? SPEAR_KNIGHT_ANI_DELAY_RIGHT : SPEAR_KNIGHT_ANI_DELAY_LEFT;
		break;

	case SPEAR_KNIGHT_STATE_WALK:
		ani = direction == Direction::Right ? SPEAR_KNIGHT_ANI_WALK_RIGHT : SPEAR_KNIGHT_ANI_WALK_LEFT;
		break;
	}

	if (!showingEffect)
	{
		animationSet->at(ani)->Render(x, y);
	}
}

void CSpearKnight::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + SPEAR_KNIGHT_BBOX_WIDTH;
	b = t + SPEAR_KNIGHT_BBOX_HEIGHT;
}

void CSpearKnight::TakeDamage(int volumes)
{
	remainingHits -= volumes;

	SetState(SPEAR_KNIGHT_STATE_DELAY);
}