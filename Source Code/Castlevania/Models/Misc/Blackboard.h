#pragma once
#include <vector>

#include "../Characters/Players/Simon.h"
#include "../Items/Item.h"
#include "../../Utilities/Label.h"
#include "HealthBar.h"
#include "../../TileMap/TileMap.h"

class CBlackboard
{
public:
	CBlackboard(CSimon* simon);

	void SetAnimationSet(string animationSetId);
	void SetPosition(float x, float y);
	void GetPosition(float& x, float &y);

	void Update(CTileMap* tileMap);
	void Render();

	~CBlackboard();

protected:
	CSimon* simon;

	float x;
	float y;

	CAnimationSet* animationSet;

	CLabel* scoreLabel;
	CLabel* timeLabel;
	CLabel* stageLabel;
	CLabel* heartsLabel;
	CLabel* livesLabel;

	CHealthBar* simonHealthBar;
	CHealthBar* enemyHealthBar;

	CItem* subWeapon;

	void RenderLabels();
	void RenderHealthBars();

	string padZero(int value, int numberOfZeros);
};

