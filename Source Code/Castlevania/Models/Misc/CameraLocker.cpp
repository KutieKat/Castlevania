#include "CameraLocker.h"

CCameraLocker::CCameraLocker()
{
	SetAnimationSet("transparency");
}

void CCameraLocker::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CCameraLocker::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("CAMERA_LOCKER_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("CAMERA_LOCKER_BBOX_HEIGHT");
}