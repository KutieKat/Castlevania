#include "Crown.h"

CCrown::CCrown()
{
	isItem = true;
	SetAnimationSet("crown");
}

void CCrown::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	y += vy * dt;

	if (y < topBound)
	{
		vy = 0;
	}
}

void CCrown::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_APPEAR:
		vy = -MONEY_BAG_MOVE_UP_SPEED;
		break;
	}
}

void CCrown::Render()
{
	if (!showingEndingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CCrown::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEndingEffect)
	{
		left = x;
		top = y;
		right = left + CROWN_BBOX_WIDTH;
		bottom = top + CROWN_BBOX_HEIGHT;
	}
}