#include "TopStair.h"

CTopStair::CTopStair()
{
	SetAnimationSet("transparency");
}

void CTopStair::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CTopStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + TOP_STAIR_BBOX_WIDTH;
	bottom = top + TOP_STAIR_BBOX_HEIGHT;
}