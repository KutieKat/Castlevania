#include "OneHundred.h"

COneHundred::COneHundred()
{
	isEffect = true;
	SetAnimationSet("one_hundred");
}

void COneHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void COneHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
