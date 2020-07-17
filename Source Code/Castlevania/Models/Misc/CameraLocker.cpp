#include "CameraLocker.h"
#include "../../Game.h"

CCameraLocker::CCameraLocker()
{
	SetAnimationSet("transparency");
}

void CCameraLocker::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CCameraLocker::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + CAMERA_LOCKER_BBOX_WIDTH;
	bottom = top + CAMERA_LOCKER_BBOX_HEIGHT;
}