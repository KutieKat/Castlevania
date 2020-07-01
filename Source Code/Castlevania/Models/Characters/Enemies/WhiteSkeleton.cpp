#include <cstdlib>
#include <time.h>

#include "WhiteSkeleton.h"
#include "../../../Game.h"
#include "../../Misc/Brick.h"
#include "../../Misc/TopStair.h"
#include "../../Misc/BottomStair.h"
#include "../../Misc/BreakableBrick.h"
#include "../../Weapons/HolyWaterBottle.h"
#include "../../Weapons/Bone.h"
#include "../../Weapons/WAxe.h"
#include "../../Weapons/HolyWaterBottle.h"
#include "../../Weapons/WStopwatch.h"

CWhiteSkeleton::CWhiteSkeleton(CSimon * simon)
{
	this->simon = simon;
	this->mustInArea = true;
	this->touchingGround = false;
	this->areaRadiusX = WHITE_SKELETON_AREA_RADIUS_X;
	this->areaRadiusY = WHITE_SKELETON_AREA_RADIUS_Y;
	this->jumpingCounter = 0;
	this->attackingCounter = 0;
	this->walkingCounter = 0;

	SetAnimationSet("white_skeleton");
	SetState(WHITE_SKELETON_STATE_IDLE);

	srand(time(NULL));
}

void CWhiteSkeleton::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WHITE_SKELETON_STATE_IDLE:
		vx = vy = 0;
		break;

	case WHITE_SKELETON_STATE_WALK_AROUND:
		attackingCounter = 0;
		vy = 0;
		break;

	case WHITE_SKELETON_STATE_JUMP_AROUND:
		attackingCounter = 0;
		touchingGround = false;

		vx = directionX == Direction::Right ? WHITE_SKELETON_JUMP_SPEED_X : -WHITE_SKELETON_JUMP_SPEED_X;
		vy = -WHITE_SKELETON_JUMP_SPEED_Y;
		break;

	case WHITE_SKELETON_STATE_DIE:
		vx = vy = 0;
		animationSet->at(GetAnimationToRender())->SetStartTime(GetTickCount());
		break;
	}
}

void CWhiteSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	vy += WHITE_SKELETON_GRAVITY * dt;

	if (state != WHITE_SKELETON_STATE_DIE)
	{
		directionX = x >= simon->x ? Direction::Left : Direction::Right;
	}

	if (state == WHITE_SKELETON_STATE_WALK_AROUND)
	{
		attackingCounter += 1;
		walkingCounter += 1;

		if (attackingCounter % RandomizeInterval() == 0)
		{
			Attack();
		}

		if (walkingCounter % 30 == 0)
		{
			if (state == WHITE_SKELETON_STATE_WALK_AROUND)
			{
				if (x <= simon->x)
				{
					directionX = Direction::Right;

					if (x == simon->x - WHITE_SKELETON_TO_SIMON_DISTANCE_X)
					{
						vx = 0;
					}
					else if (x < simon->x - WHITE_SKELETON_TO_SIMON_DISTANCE_X)
					{
						vx = x >= rightBound ? 0 : WHITE_SKELETON_WALK_SPEED_X;
					}
					else
					{
						vx = x <= leftBound ? 0 : -WHITE_SKELETON_WALK_SPEED_X;
					}
				}
				else
				{
					directionX = Direction::Left;

					if (x == simon->x + SIMON_BBOX_WIDTH + WHITE_SKELETON_TO_SIMON_DISTANCE_X)
					{
						vx = 0;
					}
					else if (x > simon->x + SIMON_BBOX_WIDTH + WHITE_SKELETON_TO_SIMON_DISTANCE_X)
					{
						vx = x <= leftBound ? 0 : -WHITE_SKELETON_WALK_SPEED_X;
					}
					else
					{
						vx = x >= rightBound ? 0 : WHITE_SKELETON_WALK_SPEED_X;
					}
				}
			}
		}
	}

	if (state == WHITE_SKELETON_STATE_JUMP_AROUND)
	{
		attackingCounter += 1;

		if (attackingCounter % RandomizeInterval() == 0)
		{
			Attack();
		}
	}

	if (state != WHITE_SKELETON_STATE_IDLE && state != WHITE_SKELETON_STATE_DIE)
	{
		if ((int)(y + WHITE_SKELETON_BBOX_HEIGHT) < (int)(simon->y + SIMON_BBOX_HEIGHT))
		{
			jumpingCounter += 1;

			if (jumpingCounter % 100 == 0)
			{
				if (directionX == Direction::Right)
				{
					if (x <= rightBound)
					{
						SetState(WHITE_SKELETON_STATE_JUMP_AROUND);
					}
				}
				else
				{
					if (x >= leftBound)
					{
						SetState(WHITE_SKELETON_STATE_JUMP_AROUND);
					}
				}
			}
		}
		else
		{
			if (touchingGround && state != WHITE_SKELETON_STATE_WALK_AROUND)
			{
				SetState(WHITE_SKELETON_STATE_WALK_AROUND);
			}
		}
	}

	if (state == WHITE_SKELETON_STATE_DIE && animationSet->at(GetAnimationToRender())->Over())
	{
		removable = true;
		CGame::GetInstance()->GetPlayerData()->AddScore(WHITE_SKELETON_SCORE);
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

			if (dynamic_cast<CBrick*>(e->obj))
			{
				auto brick = dynamic_cast<CBrick*>(e->obj);

				//if (brick->isGround)
				//{
					touchingGround = true;

					if (state == WHITE_SKELETON_STATE_JUMP_AROUND)
					{
						if (e->ny != 0 && rdy < 15) vx = 0;
					}
					else
					{
						if (e->nx != 0) vx = -vx;
						if (e->ny != 0) vy = 0;
					}
				//}
				//else
				//{
					if (e->nx != 0) vx = -vx;
				//}
			}
			else if (dynamic_cast<CTopStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CBottomStair*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
				if (e->ny < 0) y += dy;
			}
			else if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				if (e->nx != 0) vx = -vx;
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<WAxe*>(e->obj) || dynamic_cast<CHolyWaterBottle*>(e->obj) || dynamic_cast<WStopwatch*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;
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

void CWhiteSkeleton::Render()
{
	if (!showingEndingEffect)
	{
		animationSet->at(GetAnimationToRender())->Render(x, y);
	}
}

void CWhiteSkeleton::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		l = x;
		t = y;
		r = l + WHITE_SKELETON_BBOX_WIDTH;
		b = t + WHITE_SKELETON_BBOX_HEIGHT;
	}
}

void CWhiteSkeleton::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (state != WHITE_SKELETON_STATE_DIE)
	{
		if (attacks <= 0)
		{
			Disappear();
		}
		else
		{
			ShowEffect();
		}
	}
}

void CWhiteSkeleton::OnPlayerEnterArea()
{
	CEnemy::OnPlayerEnterArea();

	if (state == WHITE_SKELETON_STATE_IDLE)
	{
		if (y + WHITE_SKELETON_BBOX_HEIGHT < simon->y + SIMON_BBOX_HEIGHT)
		{
			SetState(WHITE_SKELETON_STATE_JUMP_AROUND);
		}
		else
		{
			SetState(WHITE_SKELETON_STATE_WALK_AROUND);
		}
	}
}

void CWhiteSkeleton::Attack()
{
	WBone* bone = new WBone();
	bone->SetPosition(x, y);
	bone->SetDisplayTime(BONE_DISPLAY_TIME);
	bone->SetDirectionX(bone->x > simon->x ? Direction::Left : Direction::Right);

	CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
	CUnit* unit = new CUnit(grid, bone, bone->x, bone->y);
}

void CWhiteSkeleton::Disappear()
{
	ShowEffect();
	SetState(WHITE_SKELETON_STATE_DIE);
}

int CWhiteSkeleton::GetAnimationToRender()
{
	int ani;

	switch (state)
	{
	case WHITE_SKELETON_STATE_IDLE:
		ani = directionX == Direction::Right ? WHITE_SKELETON_ANI_IDLE_RIGHT : WHITE_SKELETON_ANI_IDLE_LEFT;
		break;

	case WHITE_SKELETON_STATE_WALK_AROUND:
	case WHITE_SKELETON_STATE_JUMP_AROUND:
		ani = directionX == Direction::Right ? WHITE_SKELETON_ANI_WALK_RIGHT : WHITE_SKELETON_ANI_WALK_LEFT;
		break;

	case WHITE_SKELETON_STATE_DIE:
		ani = directionX == Direction::Right ? WHITE_SKELETON_ANI_DIE_RIGHT : WHITE_SKELETON_ANI_DIE_LEFT;
		break;
	}

	return ani;
}

int CWhiteSkeleton::RandomizeInterval()
{
	return rand() % (100 + 1 - 50) + 50;
}
