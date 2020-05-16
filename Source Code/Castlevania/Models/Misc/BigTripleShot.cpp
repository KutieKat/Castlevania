#include "BigTripleShot.h"

CBigTripleShot::CBigTripleShot()
{
	SetAnimationSet("big_triple_shot");
}

void CBigTripleShot::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBigTripleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BIG_TRIPLE_SHOT_BBOX_WIDTH;
	bottom = top + BIG_TRIPLE_SHOT_BBOX_HEIGHT;
}