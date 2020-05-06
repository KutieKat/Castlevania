#include "BigDoubleShot.h"

CBigDoubleShot::CBigDoubleShot()
{
	SetAnimationSet("big_double_shot");
}

void CBigDoubleShot::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBigDoubleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BIG_DOUBLE_SHOT_BBOX_WIDTH;
	bottom = top + BIG_DOUBLE_SHOT_BBOX_HEIGHT;
}