#include "HealthBar.h"

CHealthBar::CHealthBar(HealthType type, int max, int value)
{
	this->type = type;
	this->max = max;
	this->value = value;

	Update();
}

void CHealthBar::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CHealthBar::SetValue(int value)
{
	this->value = value;

	Update();
}

void CHealthBar::Render()
{
	for (int i = 0; i < volumes.size(); i++)
	{
		volumes[i]->SetPosition(this->x + i * 11, this->y);

		if (i < this->value)
		{
			switch (type)
			{
			case HealthType::Player:
				volumes[i]->SetType(HealthType::Player);
				break;

			case HealthType::Enemy:
				volumes[i]->SetType(HealthType::Enemy);
				break;

			default:
				volumes[i]->SetType(HealthType::Empty);
				break;
			}
		}

		volumes[i]->Render();
	}
}

void CHealthBar::Update()
{
	this->volumes.clear();

	for (int i = 0; i < this->max; i++) {
		CHealthVolume* healthVolume = new CHealthVolume();

		healthVolume->AddAnimation("health_volume_empty");
		healthVolume->AddAnimation("health_volume_player");
		healthVolume->AddAnimation("health_volume_enemy");

		this->volumes.push_back(healthVolume);
	}
}
