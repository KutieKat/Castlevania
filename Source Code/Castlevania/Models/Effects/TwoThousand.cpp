#include "TwoThousand.h"
#include "../../Game.h"

CTwoThousand::CTwoThousand()
{
	isEffect = true;
	SetAnimationSet("two_thousand");
	elevation = CSettingManager::GetInstance()->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CTwoThousand::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CTwoThousand::Render()
{
	animationSet->at(0)->Render(x, y);
}
