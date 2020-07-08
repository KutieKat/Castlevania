#include "CreditText.h"
#include "../../Game.h"

CCreditText::CCreditText()
{
	SetAnimationSet("credit_text");
}

void CCreditText::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CCreditText::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}