#include "FourHundred.h"

CFourHundred::CFourHundred()
{
	isEffect = true;
	SetAnimationSet("four_hundred");
}

void CFourHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CFourHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
