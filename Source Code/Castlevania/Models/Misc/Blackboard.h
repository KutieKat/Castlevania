#pragma once
#include <vector>

#include "../Characters/Players/Simon.h"
#include "../../Utilities/Label.h"
#include "HealthBar.h"
#include "../../TileMap/TileMap.h"

class CBlackboard
{
public:
	CBlackboard();

	void SetAnimationSet(string animationSetId);
	void SetPosition(float x, float y);
	void GetPosition(float& x, float &y);

	void Update();
	void Render();

	void SetPlayer(CSimon* simon);
	void SetTileMap(CTileMap* tileMap) { this->tileMap = tileMap; }

	~CBlackboard();

protected:
	CSimon* simon;
	CTileMap* tileMap;

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

	CGameObject* subWeapon;

	void RenderLabels();
	void RenderHealthBars();

	string padZero(int value, int numberOfZeros);
};

