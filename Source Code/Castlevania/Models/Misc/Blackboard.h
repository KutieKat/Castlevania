#pragma once
#include <vector>

#include "../../Game.h"
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
	void SetTileMap(CTileMap* tileMap);

	~CBlackboard();

protected:
	CGame* game;
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
	CGameObject* whipPower;

	void InitLabels();
	void InitHealthBars();
	void InitSubWeapon();
	void InitWhipPower();

	void RenderLabels();
	void RenderHealthBars();
	void RenderSubWeapon();
	void RenderWhipPowerType();

	void UpdatePosition();
	void UpdateLabels();
	void UpdateHealthBars();
	void UpdateSubWeapon();
	void UpdateWhipPowerType();

	string padZero(int value, int numberOfZeros);
	string currentSubWeaponType;

	int currentWhipPower;
};

