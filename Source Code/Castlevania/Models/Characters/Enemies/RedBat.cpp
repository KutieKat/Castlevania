#include "RedBat.h"
#include "../../../Game.h"

CRedBat::CRedBat(CSimon* simon)
{
	this->simon = simon;
	this->mustInArea = true;
	this->areaRadiusX = RED_BAT_AREA_RADIUS_X;
	this->areaRadiusY = RED_BAT_AREA_RADIUS_Y;
	this->flyingCounter = 0;

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
		vx = directionX == Direction::Right ? RED_BAT_MOVE_SPEED_X : -RED_BAT_MOVE_SPEED_X;
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
		x += (directionX == Direction::Right ? vx : -vx) * dt;
		y += (y < simon->y + 10 ? RED_BAT_MOVE_SPEED_Y : 0) * dt;
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
		if (CGame::GetInstance()->BoundingBoxDisplayed())
		{
			RenderBoundingBox();
		}

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
	CEnemy::OnPlayerEnterArea();

	SetState(RED_BAT_STATE_MOVE);
}
