#include "TripleShot.h"

CTripleShot::CTripleShot()
{
	isItem = true;
	SetAnimationSet("triple_shot");
}

void CTripleShot::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CTripleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + TRIPLE_SHOT_BBOX_WIDTH;
	bottom = top + TRIPLE_SHOT_BBOX_HEIGHT;
}