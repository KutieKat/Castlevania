#include "BigCandle.h"
#include "../../Utilities/SafeDelete.h"
#include "../../Utilities/Debug.h"
#include "../../Models/Misc/Brick.h"

CBigCandle::CBigCandle()
{
	this->showingEffect = false;

	SetAnimationSet("big_candle");
}

void CBigCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += BIG_CANDLE_GRAVITY * dt;

	if (this->endingEffect && this->endingEffect->Over())
	{
		if (this->hiddenItem)
		{
			this->hiddenItem->SetVisibility(Visibility::Visible);
			this->hiddenItem->SetDisplayTime(ITEM_DISPLAY_TIME);
		}

		this->visibility = Visibility::Hidden;
	}

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

			if (dynamic_cast<CBrick*>(e->obj))
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
	if (!showingEffect)
	{
		left = x;
		top = y;
		right = left + BIG_CANDLE_BBOX_WIDTH;
		bottom = top + BIG_CANDLE_BBOX_HEIGHT;
	}
}

void CBigCandle::Disappear()
{
	this->showingEffect = true;

	if (this->endingEffect)
	{
		this->endingEffect->SetPosition(x, y);
		this->endingEffect->SetStartTime(GetTickCount());
	}
}

void CBigCandle::SetEndingEffect(CEffect* effect)
{
	this->endingEffect = effect;
}

void CBigCandle::SetHiddenItem(CItem* item)
{
	this->hiddenItem = item;
	this->hiddenItem->SetVisibility(Visibility::Hidden);
}

CBigCandle::~CBigCandle()
{
	SAFE_DELETE(this->endingEffect);
	SAFE_DELETE(this->hiddenItem);
}
