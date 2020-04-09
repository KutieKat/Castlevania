#include "Weapon.h"

void CWeapon::Upgrade()
{
	this->level++;
}

int CWeapon::GetLevel()
{
	return this->level;
}
