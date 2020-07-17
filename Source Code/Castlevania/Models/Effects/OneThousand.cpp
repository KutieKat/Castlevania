#include "OneThousand.h"
#include "../../Game.h"

COneThousand::COneThousand()
{
	isEffect = true;
	SetAnimationSet("one_thousand");
	elevation = CSettingManager::GetInstance()->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void COneThousand::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void COneThousand::Render()
{
	animationSet->at(0)->Render(x, y);
}
