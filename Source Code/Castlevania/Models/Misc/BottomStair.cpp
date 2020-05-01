#include "BottomStair.h"

CBottomStair::CBottomStair()
{
	SetAnimationSet("transparency");
}

void CBottomStair::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBottomStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOTTOM_STAIR_BBOX_WIDTH;
	bottom = top + BOTTOM_STAIR_BBOX_HEIGHT;
}