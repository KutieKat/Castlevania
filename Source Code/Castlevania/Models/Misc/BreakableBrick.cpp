#include "BreakableBrick.h"
#include "../../Utilities/Debug.h"

CBreakableBrick::CBreakableBrick()
{
	isGround = false;
	SetAnimationSet("breakable_brick");
}

void CBreakableBrick::Render()
{
	if (!showingEndingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CBreakableBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

		left = x;
		top = y;
		right = left + settingManager->GetIntValue("BREAKABLE_BRICK_BBOX_WIDTH");
		bottom = top + settingManager->GetIntValue("BREAKABLE_BRICK_BBOX_HEIGHT");
	}
}