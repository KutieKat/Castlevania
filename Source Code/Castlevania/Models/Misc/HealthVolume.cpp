#include "HealthVolume.h"
#include "../../Animations/AnimationManager.h"
#include "../../Animations/AnimationSets.h"
#include "../../Utilities/Debug.h"

CHealthVolume::CHealthVolume()
{
	SetAnimationSet("health_volume");
}

void CHealthVolume::SetAnimationSet(string animationSetId)
{
	animationSet = CAnimationSets::GetInstance()->Get(animationSetId);
}

void CHealthVolume::Render()
{
	int ani;

	switch (type)
	{
	case HealthType::Empty:
		ani = HEALTH_VOLUME_ANI_EMPTY;
		break;

	case HealthType::Player:
		ani = HEALTH_VOLUME_ANI_PLAYER;
		break;

	case HealthType::Enemy:
		ani = HEALTH_VOLUME_ANI_ENEMY;
		break;
	}

	animationSet->at(ani)->Render(x, y);
}

void CHealthVolume::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CHealthVolume::SetType(HealthType type)
{
	this->type = type;
}
