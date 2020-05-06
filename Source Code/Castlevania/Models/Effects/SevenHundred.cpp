#include "SevenHundred.h"

CSevenHundred::CSevenHundred()
{
	isEffect = true;
	SetAnimationSet("seven_hundred");
}

void CSevenHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CSevenHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
