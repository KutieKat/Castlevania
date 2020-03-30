#include "Simon.h"
#include "../../Misc/BigCandle.h"
#include "../../Misc/Brick.h"
#include "../../../Utilities/Debug.h"

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += SIMON_GRAVITY * dt;

	if (y < 50)
	{
		y = 50;
	}

	//if (x > 1000)
	//{
	//	SetState(SIMON_STATE_AUTO_WALKING_RIGHT);
	//}

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

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				jumping = false;
			}
			else if (dynamic_cast<CBigCandle*>(e->obj))
			{
				x -= nx * 0.4f;
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
	int ani;

	if (state == SIMON_STATE_JUMPING)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_JUMPING_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_JUMPING_LEFT;
		}
	}
	else if (state == SIMON_STATE_SITTING)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_SITTING_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_SITTING_LEFT;
		}
	}
	else if (state == SIMON_STATE_STANDING_AND_ATTACKING)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_STANDING_RIGHT_AND_HITTING;
		}
		else
		{
			ani = SIMON_ANI_STANDING_LEFT_AND_HITTING;
		}
	}
	else if (state == SIMON_STATE_SITTING_AND_ATTACKING)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_SITTING_RIGHT_AND_HITTING;
		}
		else
		{
			ani = SIMON_ANI_SITTING_LEFT_AND_HITTING;
		}
	}
	else if (state == SIMON_STATE_IDLE)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_IDLE_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_IDLE_LEFT;
		}
	}
	else
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_WALKING_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_WALKING_LEFT;
		}
	}

	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;

	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;

	case SIMON_STATE_AUTO_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;

	case SIMON_STATE_AUTO_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;

	case SIMON_STATE_SITTING:
		vx = 0;
		sitting = true;
		break;

	case SIMON_STATE_JUMPING:
		jumping = true;
		vy = -SIMON_JUMP_SPEED;
		break;

	case SIMON_STATE_SITTING_AND_ATTACKING:
		attacking = true;
		vx = 0;
		break;

	case SIMON_STATE_STANDING_AND_ATTACKING:
		attacking = true;
		vx = 0;
		break;

	case SIMON_STATE_IDLE:
		vx = 0;
		sitting = false;
		attacking = false;
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

CBoundingBox CSimon::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}
