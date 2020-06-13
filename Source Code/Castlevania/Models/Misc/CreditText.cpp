#include "CreditText.h"

CCreditText::CCreditText()
{
	SetAnimationSet("credit_text");
	animationSet->at(0)->SetStartTime(GetTickCount());
}

void CCreditText::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CCreditText::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}