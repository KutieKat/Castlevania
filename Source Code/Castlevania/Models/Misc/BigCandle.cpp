#include "BigCandle.h"
#include "../../Models/Misc/Ground.h"

CBigCandle::CBigCandle()
{
	comeThrough = true;
	SetAnimationSet("big_candle");
}

void CBigCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += BIG_CANDLE_GRAVITY * dt;

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
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGround*>(e->obj))
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
		}
	}
}

void CBigCandle::Render()
{
	if (!showingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CBigCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BIG_CANDLE_BBOX_WIDTH;
	bottom = top + BIG_CANDLE_BBOX_HEIGHT;
}

void CBigCandle::Disappear()
{
	this->showingEffect = true;

	if (this->endingEffect)
	{
		this->endingEffect->SetVisibility(Visibility::Visible);
		this->endingEffect->SetDisplayTime(EFFECT_DISPLAY_TIME);
	}
}
