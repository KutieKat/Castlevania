#include "Ghost.h"
#include "../../../Game.h"
#include "../../Misc/BottomStair.h"
#include "../../Weapons/HolyWaterBottle.h"

CGhost::CGhost(CSimon* simon)
{
	this->simon = simon;

	SetAnimationSet("ghost");
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

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

	x += (directionX == Direction::Right ? GHOST_MOVE_SPEED : -GHOST_MOVE_SPEED) * dt;
	y += (y < simon->y + 20 ? GHOST_MOVE_SPEED : -GHOST_MOVE_SPEED) * dt;
}

void CGhost::Render()
{
	int ani = directionX == Direction::Right ? GHOST_ANI_MOVE_RIGHT : GHOST_ANI_MOVE_LEFT;

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
	}
}