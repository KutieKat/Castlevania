#include "Ghoul.h"
#include "../../Misc/BottomStair.h"
#include "../../Misc/Brick.h"
#include "../../../Game.h"

CGhoul::CGhoul(CSimon* simon)
{
	this->simon = simon;

	SetAnimationSet("ghoul");
}

void CGhoul::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

    dx = (directionX == Direction::Right ? GHOUL_MOVE_SPEED : -GHOUL_MOVE_SPEED) * dt;

    vy += SPEAR_KNIGHT_GRAVITY * dt;

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

			if (dynamic_cast<CBrick*>(e->obj))
			{
				auto brick = dynamic_cast<CBrick*>(e->obj);

				//if (brick->isGround)
				//{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) vy = 0;
				//}
			}
			else if (dynamic_cast<CBottomStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
				if (e->ny < 0) y += dy;
			}
			else
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}
	}
}

void CGhoul::Render()
{
	int ani = directionX == Direction::Right ? GHOUL_ANI_MOVE_RIGHT : GHOUL_ANI_MOVE_LEFT;

	if (!showingEndingEffect)
	{
		if (CGame::GetInstance()->BoundingBoxDisplayed())
		{
			RenderBoundingBox();
		}

		animationSet->at(ani)->Render(x, y);
	}
}

void CGhoul::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		l = x;
		t = y;
		r = l + GHOUL_BBOX_WIDTH;
		b = t + GHOUL_BBOX_HEIGHT;
	}
}

void CGhoul::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(GHOUL_SCORE);
	}
	else
	{
		ShowEffect();
	}
}