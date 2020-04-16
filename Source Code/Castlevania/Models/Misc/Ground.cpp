#include "Ground.h"

CGround::CGround()
{
	SetAnimationSet("ground");
}

void CGround::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CGround::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + GROUND_BBOX_WIDTH;
	bottom = top + GROUND_BBOX_HEIGHT;
}