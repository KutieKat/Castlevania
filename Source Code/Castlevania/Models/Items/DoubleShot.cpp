#include "DoubleShot.h"

CDoubleShot::CDoubleShot()
{
	isItem = true;
	SetAnimationSet("double_shot");
}

void CDoubleShot::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CDoubleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + DOUBLE_SHOT_BBOX_WIDTH;
	bottom = top + DOUBLE_SHOT_BBOX_HEIGHT;
}