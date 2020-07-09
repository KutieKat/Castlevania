#include "Bone.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Misc/Brick.h"
#include "../Misc/BottomStair.h"

WBone::WBone()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();
	SetAnimationSet("bone");

	elevation = settingManager->GetIntValue("WEAPON_DEFAULT_ELEVATION");
	vy = settingManager->GetFloatValue("BONE_MOVE_SPEED_Y");
}

void WBone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	vx = directionX == Direction::Right ? settingManager->GetFloatValue("BONE_MOVE_SPEED_X") : -settingManager->GetFloatValue("BONE_MOVE_SPEED_X");

	vy += settingManager->GetFloatValue("BONE_GRAVITY") * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (y < CGame::GetInstance()->GetCamera()->GetTop() || y > CGame::GetInstance()->GetCamera()->GetBottom() || x > CGame::GetInstance()->GetCamera()->GetRight() || x < CGame::GetInstance()->GetCamera()->GetLeft())
		{
			removable = true;
		}
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

			//if (dynamic_cast<CGround*>(e->obj))
			//{
			//	removable = true;
			//}
			//else if (dynamic_cast<CBrick*>(e->obj))
			//{
			//	auto brick = dynamic_cast<CBrick*>(e->obj);

			//	if (brick->isGround)
			//	{
			//		removable = true;
			//	}
			//	else
			//	{
			//		if (e->nx != 0) x += dx;
			//		if (e->ny != 0) y += dy;
			//	}
			//}
			//else
			//{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			//}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void WBone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("BONE_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("BONE_BBOX_HEIGHT");
}

void WBone::Render()
{
	animationSet->at(0)->Render(x, y);
}
