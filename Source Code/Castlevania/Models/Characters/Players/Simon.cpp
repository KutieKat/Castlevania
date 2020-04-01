#include "Simon.h"
#include "../../Misc/BigCandle.h"
#include "../../Misc/Brick.h"
#include "../../../Utilities/Debug.h"

CSimon::CSimon()
{
	sitting = false;
	standing = false;
	animationFinished = false;

	whip = new CWhip();
	whip->AddAnimation("whip_level_1_left");
	whip->AddAnimation("whip_level_1_right");
	whip->SetLevel(WHIP_LEVEL_1);
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += SIMON_GRAVITY * dt;
	whip->SetDirection(direction);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				standing = true;

				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<CBigCandle*>(e->obj))
			{
				if (e->ny != 0) y += dy;
				if (e->nx != 0) x += dx;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CSimon::Render()
{
	int alpha = 255;
	animations[GetAnimationToRender()]->Render(animationFinished, x, y, alpha);
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALK:
		vx = direction == Direction::Right ? SIMON_WALK_SPEED : -SIMON_WALK_SPEED;
		break;

	case SIMON_STATE_JUMP:
		if (standing)
		{
			vy = -SIMON_JUMP_SPEED;
			standing = false;
		}

		break;

	case SIMON_STATE_SIT:
		sitting = true;
		vx = 0;
		break;

	case SIMON_STATE_SIT_AND_ATTACK:
		animationFinished = false;
		vx = 0;
		break;

	case SIMON_STATE_STAND_AND_ATTACK:
		animationFinished = false;
		vx = 0;
		break;

	case SIMON_STATE_IDLE:
		sitting = false;
		vx = 0;
		break;
	}
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}

int CSimon::GetAnimationToRender()
{
	int ani;

	if (state == SIMON_STATE_WALK)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_WALK_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_WALK_LEFT;
		}
	}
	else if (state == SIMON_STATE_SIT)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_SIT_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_SIT_LEFT;
		}
	}
	else if (state == SIMON_STATE_JUMP)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_JUMP_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_JUMP_LEFT;
		}
	}
	else if (state == SIMON_STATE_SIT_AND_ATTACK)
	{
		whip->SetPosition(x - 90, y + 14);

		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_SIT_RIGHT_AND_ATTACK;
		}
		else
		{
			ani = SIMON_ANI_SIT_LEFT_AND_ATTACK;
		}

		whip->Render();
	}
	else if (state == SIMON_STATE_STAND_AND_ATTACK)
	{
		whip->SetPosition(x - 90, y);

		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_STAND_RIGHT_AND_ATTACK;
		}
		else
		{
			ani = SIMON_ANI_STAND_LEFT_AND_ATTACK;
		}

		whip->Render();
	}
	else
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_IDLE_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_IDLE_LEFT;
		}
	}

	return ani;
}

CBoundingBox CSimon::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}

void CSimon::ResetAnimations()
{
	for (int i = 0; i < animations.size(); i++)
	{
		animations[i]->Reset();
	}

	whip->ResetAnimations();
}

bool CSimon::Sitting()
{
	return this->sitting;
}

bool CSimon::Standing()
{
	return this->standing;
}

bool CSimon::AnimationFinished()
{
	return this->animationFinished;
}
