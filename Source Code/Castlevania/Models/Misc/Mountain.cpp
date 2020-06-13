#include "Mountain.h"

CMountain::CMountain()
{
	SetAnimationSet("mountain");

	elevation = 2;
}

void CMountain::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CMountain::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}