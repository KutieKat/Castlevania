#include "RedBat.h"
#include "../../../Game.h"

CRedBat::CRedBat()
{
	mustInArea = true;
	areaRadius = RED_BAT_AREA_RADIUS;

	SetAnimationSet("red_bat");
	SetState(RED_BAT_STATE_STATIC);
}

void CRedBat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case RED_BAT_STATE_STATIC:
		vx = 0;
		break;

	case RED_BAT_STATE_MOVE:
		vx = directionX == Direction::Right ? RED_BAT_MOVE_SPEED : -RED_BAT_MOVE_SPEED;
		break;
	}
}

void CRedBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	if (state == RED_BAT_STATE_MOVE)
	{
		x += vx * dt;
		y += sin(x * PI / 180);
	}
}

void CRedBat::Render()
{
	int ani;
	
	switch (state)
	{
	case RED_BAT_STATE_STATIC:
		ani = directionX == Direction::Right ? RED_BAT_ANI_STATIC_RIGHT : RED_BAT_ANI_STATIC_LEFT;
		break;
	
	case RED_BAT_STATE_MOVE:
		ani = directionX == Direction::Right ? RED_BAT_ANI_MOVE_RIGHT : RED_BAT_ANI_MOVE_LEFT;
		break;
	}
	
	if (!showingEndingEffect)
	{
		animationSet->at(ani)->Render(x, y);
	}
}

void CRedBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!showingEndingEffect)
	{
		left = x;
		top = y;
		right = left + RED_BAT_BBOX_WIDTH;
		bottom = top + RED_BAT_BBOX_HEIGHT;
	}
}

void CRedBat::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(RED_BAT_SCORE);
	}
	else
	{
		ShowEffect();
	}
}

void CRedBat::OnPlayerEnterArea()
{
	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();
	
	if (softPaused) return;

	SetState(RED_BAT_STATE_MOVE);
}
