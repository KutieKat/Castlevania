#include "BiStair.h"

CBiStair::CBiStair()
{
	SetAnimationSet("transparency");
}

void CBiStair::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBiStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BI_STAIR_BBOX_WIDTH;
	bottom = top + BI_STAIR_BBOX_HEIGHT;
}