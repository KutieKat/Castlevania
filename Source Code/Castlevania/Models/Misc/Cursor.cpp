#include "Cursor.h"
#include "../../Game.h"

CCursor::CCursor()
{
	SetAnimationSet("big_heart");
}

void CCursor::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CCursor::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}