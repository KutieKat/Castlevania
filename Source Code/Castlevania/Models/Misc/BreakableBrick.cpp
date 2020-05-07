#include "BreakableBrick.h"
#include "../../Utilities/Debug.h"

CBreakableBrick::CBreakableBrick()
{
	isGround = false;
	SetAnimationSet("breakable_brick");
}

void CBreakableBrick::Render()
{
	if (!showingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CBreakableBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEffect)
	{
		left = x;
		top = y;
		right = left + BREAKABLE_BRICK_BBOX_WIDTH;
		bottom = top + BREAKABLE_BRICK_BBOX_HEIGHT;
	}
}