#include "HealthBar.h"
#include "../../Utilities/Debug.h"

CHealthBar::CHealthBar(HealthType type, int max, int value)
{
	this->type = type;
	this->max = max;
	this->value = value;

	for (int i = 0; i < this->max; i++) {
		CHealthVolume* healthVolume = new CHealthVolume();
		this->volumes.emplace_back(healthVolume);
	}
}

void CHealthBar::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CHealthBar::SetValue(int value)
{
	this->value = value;
}

void CHealthBar::Render()
{
	for (int i = 0; i < volumes.size(); i++)
	{
		volumes[i]->Render();
	}
}

void CHealthBar::Update()
{
	for (int i = 0; i < volumes.size(); i++)
	{
		volumes[i]->SetPosition(x + i * 11, y);

		if (i < value)
		{
			switch (type)
			{
			case HealthType::Player:
				volumes[i]->SetType(HealthType::Player);
				break;

			case HealthType::Enemy:
				volumes[i]->SetType(HealthType::Enemy);
				break;
			}
		}
		else
		{
			volumes[i]->SetType(HealthType::Empty);
		}
	}
}
