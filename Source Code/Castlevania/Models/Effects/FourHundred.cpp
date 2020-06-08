#include "FourHundred.h"

CFourHundred::CFourHundred()
{
	isEffect = true;
	SetAnimationSet("four_hundred");
	elevation = EFFECT_DEFAULT_ELEVATION;
}

void CFourHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CFourHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
