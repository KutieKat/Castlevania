#pragma once
#include <string>

#include "../GameObject.h"
#include "../Characters/Players/Simon.h"
#include "../../Utilities/Constants.h"
#include "../Characters/Enemies/Enemy.h"
using namespace std;

class CEnemySpawnerArea : public CGameObject
{
public:
	CEnemySpawnerArea(CSimon* simon);

	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SpawnEnemy();
	void CreateEnemy();
	void SetEnemyType(string type);
	void SetInterval(DWORD interval);
	void SetSpawningPosition(string position);

protected:
	CSimon* simon;
	CEnemy* spawnedEnemy;

	DWORD nextSpawningTime;
	DWORD spawningInterval;

	string enemyType;
	string spawningPosition;
};