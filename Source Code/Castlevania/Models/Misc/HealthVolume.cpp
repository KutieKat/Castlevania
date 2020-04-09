#include "HealthVolume.h"
#include "../../Animations/AnimationManager.h"

void CHealthVolume::AddAnimation(string aniId)
{
	CAnimation* ani = CAnimationManager::GetInstance()->Get(aniId);
	animations.push_back(ani);
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

	this->animations[ani]->Render(x, y);
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
