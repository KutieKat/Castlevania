#include "SpearKnight.h"
#include "../../../Game.h"
#include "../../Misc/BottomStair.h"

CSpearKnight::CSpearKnight()
{
	SetAnimationSet("spear_knight");

	attacks = SPEAR_KNIGHT_ATTACKS;
	SetState(SPEAR_KNIGHT_STATE_WALK);
}

void CSpearKnight::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SPEAR_KNIGHT_STATE_WALK:
		vx = directionX == Direction::Right ? SPEAR_KNIGHT_WALK_SPEED : -SPEAR_KNIGHT_WALK_SPEED;
		break;

	case SPEAR_KNIGHT_STATE_DELAY:
		vx = 0;
		delayTimeout = GetTickCount() + ENEMY_DELAY_TIME;
		break;
	}
}

void CSpearKnight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += SPEAR_KNIGHT_GRAVITY * dt;

	if (delayTimeout != -1 && GetTickCount() > delayTimeout)
	{
		delayTimeout = -1;
		attacks = SPEAR_KNIGHT_ATTACKS;

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

		if (x <= leftBound)
		{
			directionX = Direction::Right;
			vx = -vx;
		}
		else if (x >= rightBound)
		{
			directionX = Direction::Left;
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
		ani = directionX == Direction::Right ? SPEAR_KNIGHT_ANI_DELAY_RIGHT : SPEAR_KNIGHT_ANI_DELAY_LEFT;
		break;

	case SPEAR_KNIGHT_STATE_WALK:
		ani = directionX == Direction::Right ? SPEAR_KNIGHT_ANI_WALK_RIGHT : SPEAR_KNIGHT_ANI_WALK_LEFT;
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

void CSpearKnight::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(SPEAR_KNIGHT_SCORE);
	}
	else
	{
		SetState(SPEAR_KNIGHT_STATE_DELAY);
	}
}