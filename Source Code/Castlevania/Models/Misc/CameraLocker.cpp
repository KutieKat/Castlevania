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
	left = x;
	top = y;
	right = left + CAMERA_LOCKER_BBOX_WIDTH;
	bottom = top + CAMERA_LOCKER_BBOX_HEIGHT;
}