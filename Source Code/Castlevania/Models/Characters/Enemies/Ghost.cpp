#include "Ghost.h"
#include "../../../Game.h"
#include "../../Misc/BottomStair.h"
#include "../../Weapons/HolyWaterBottle.h"

CGhost::CGhost(CSimon* simon)
{
	this->simon = simon;
	this->attacks = GHOST_ATTACKS;

	SetAnimationSet("ghost");
	SetState(GHOST_STATE_MOVE);
}

void CGhost::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GHOST_STATE_MOVE:
		vx = GHOST_MOVE_SPEED_X;
		vy = GHOST_MOVE_SPEED_Y;
		break;

	case GHOST_STATE_DELAY:
		vx = vy = 0;
		delayTimeout = GetTickCount() + ENEMY_DELAY_TIME;
		break;
	}
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	if (state == GHOST_STATE_DELAY && delayTimeout != -1 && GetTickCount() > delayTimeout)
	{
		if (GetTickCount() > delayTimeout + ENEMY_DELAY_PLUS_TIME)
		{
			delayTimeout = -1;
		}

		SetState(GHOST_STATE_MOVE);
	}

	if (directionX == Direction::Left)
	{
		if (x < CGame::GetInstance()->GetCamera()->GetLeft())
		{
			removable = true;
		}
	}
	else if (directionX == Direction::Right)
	{
		if (x > CGame::GetInstance()->GetCamera()->GetRight())
		{
			removable = true;
		}
	}

	x += (directionX == Direction::Right) ? vx : -vx * dt;

	if (x > simon->x)
	{
		if (y < simon->y + 20)
		{
			y += ceil(vy * dt);
		}
		else
		{
			y -= ceil(vy * dt);
		}
	}
	else
	{
		if (y < simon->y + 20)
		{
			y += ceil(vy * dt);
		}
	}
}

void CGhost::Render()
{
	int ani;

	switch (state)
	{
	case GHOST_STATE_MOVE:
		ani = directionX == Direction::Right ? GHOST_ANI_MOVE_RIGHT : GHOST_ANI_MOVE_LEFT;
		break;

	case GHOST_STATE_DELAY:
		ani = directionX == Direction::Right ? GHOST_ANI_DELAY_RIGHT : GHOST_ANI_DELAY_LEFT;
		break;
	}

	if (!showingEndingEffect)
	{
		animationSet->at(ani)->Render(x, y);
	}
}

void CGhost::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		l = x;
		t = y;
		r = l + GHOST_BBOX_WIDTH;
		b = t + GHOST_BBOX_HEIGHT;
	}
}

void CGhost::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(GHOST_SCORE);
	}
	else
	{
		ShowEffect();
		SetState(GHOST_STATE_DELAY);
	}
}