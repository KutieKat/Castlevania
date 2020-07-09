#include "MovingBar.h"
#include "Brick.h"

CMovingBar::CMovingBar()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	vx = settingManager->GetFloatValue("MOVING_BAR_MOVE_SPEED");

	SetAnimationSet("moving_bar");
}

void CMovingBar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

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
				vx = -vx;
			}
		}
	}
}

void CMovingBar::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CMovingBar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetFloatValue("MOVING_BAR_BBOX_WIDTH");
	bottom = top + settingManager->GetFloatValue("MOVING_BAR_BBOX_HEIGHT");
}