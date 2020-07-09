#include "TwoThousand.h"

CTwoThousand::CTwoThousand()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	isEffect = true;
	SetAnimationSet("two_thousand");
	elevation = settingManager->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CTwoThousand::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CTwoThousand::Render()
{
	animationSet->at(0)->Render(x, y);
}
