#include "Blackboard.h"
#include "../../Utilities/Debug.h"
#include "../Items/Dagger.h"
#include "../../../Utilities/SafeDelete.h"
#include "../../Game.h"
#include "../../Animations/AnimationSets.h"
#include "../ObjectFactory.h"

CBlackboard::CBlackboard()
{
	x = y = 0;
	game = CGame::GetInstance();

	InitSubWeapon();
	InitWhipPower();
	InitLabels();
	InitHealthBars();

	SetAnimationSet("blackboard");
}

void CBlackboard::SetAnimationSet(string animationSetId)
{
	animationSet = CAnimationSets::GetInstance()->Get(animationSetId);
}

void CBlackboard::Update()
{
	UpdatePosition();
	UpdateLabels();
	UpdateHealthBars();
	UpdateSubWeapon();
	UpdateSubWeapon();
	UpdateWhipPowerType();
}

void CBlackboard::Render()
{
	animationSet->at(0)->Render(x, y);

	RenderLabels();
	RenderHealthBars();
	RenderSubWeapon();
	RenderWhipPowerType();
}

void CBlackboard::SetPlayer(CSimon* simon)
{
	this->simon = simon;
}

void CBlackboard::SetTileMap(CTileMap* tileMap)
{
	this->tileMap = tileMap;
}

string CBlackboard::padZero(int value, int numberOfZeros)
{
	int numZeros = numberOfZeros;
	int valueLength = to_string(value).length();
	string strValue = to_string(value);

	if (valueLength > numZeros)
	{
		return strValue;
	}

	return string(numZeros - valueLength, '0') + strValue;
}

void CBlackboard::InitLabels()
{
	CPlayerData* playerData = game->GetPlayerData();

	string score = padZero(playerData->GetScore(), 6);
	string remainingTime = padZero(game->GetTimer()->GetRemainingTime(), 4);
	string stage = game->GetSceneManager()->GetCurrentScene()->GetStage();
	string hearts = padZero(playerData->GetHearts(), 2);
	string lives = padZero(playerData->GetLives(), 2);

	scoreLabel = new CLabel(score, 103, 15);
	timeLabel = new CLabel(remainingTime, 288, 15);
	stageLabel = new CLabel(stage, 460, 15);
	heartsLabel = new CLabel(hearts, 394, 34, 18);
	livesLabel = new CLabel(lives, 394, 51, 18);
}

void CBlackboard::InitHealthBars()
{
	CPlayerData* playerData = game->GetPlayerData();
	CBossData* bossData = game->GetBossData();
	CSettingManager* settingManager = CSettingManager::GetInstance();

	simonHealthBar = new CHealthBar(HealthType::Player, settingManager->GetIntValue("SIMON_DEFAULT_HEALTHS"), playerData->GetHealthVolumes());
	enemyHealthBar = new CHealthBar(HealthType::Enemy, settingManager->GetIntValue("BOSS_DEFAULT_HEALTHS"), bossData->GetHealthVolumes());
}

void CBlackboard::InitSubWeapon()
{
	CPlayerData* playerData = game->GetPlayerData();

	currentSubWeaponType = playerData->GetSubWeaponType();
	subWeapon = CObjectFactory::Construct(currentSubWeaponType);
}

void CBlackboard::InitWhipPower()
{
	currentWhipPower = game->GetPlayerData()->GetPower();

	switch (game->GetPlayerData()->GetPower())
	{
	case DOUBLE_POWER:
		whipPower = new CBigDoubleShot();
		break;

	case TRIPLE_POWER:
		whipPower = new CBigTripleShot();
		break;
	}
}

void CBlackboard::RenderLabels()
{
	scoreLabel->Render();
	timeLabel->Render();
	stageLabel->Render();
	heartsLabel->Render();
	livesLabel->Render();
}

void CBlackboard::RenderHealthBars()
{
	simonHealthBar->Render();
	enemyHealthBar->Render();
}

void CBlackboard::RenderSubWeapon()
{
	if (subWeapon)
	{
		subWeapon->Render();
	}
}

void CBlackboard::RenderWhipPowerType()
{
	if (whipPower)
	{
		whipPower->Render();
	}
}

void CBlackboard::UpdatePosition()
{
	x = CGame::GetInstance()->GetCamera()->GetLeft();
}

void CBlackboard::UpdateLabels()
{
	CPlayerData* playerData = game->GetPlayerData();

	string score = padZero(playerData->GetScore(), 6);
	string remainingTime = padZero(game->GetTimer()->GetRemainingTime(), 4);
	string hearts = padZero(playerData->GetHearts(), 2);

	scoreLabel->SetText(score);
	timeLabel->SetText(remainingTime);
	heartsLabel->SetText(hearts);
}

void CBlackboard::UpdateHealthBars()
{
	CPlayerData* playerData = game->GetPlayerData();
	CBossData* bossData = game->GetBossData();

	simonHealthBar->SetValue(playerData->GetHealthVolumes());
	simonHealthBar->SetPosition(x + 110, y + 32);
	simonHealthBar->Update();

	enemyHealthBar->SetValue(bossData->GetHealthVolumes());
	enemyHealthBar->SetPosition(x + 110, y + 50);
	enemyHealthBar->Update();
}

void CBlackboard::UpdateSubWeapon()
{
	CPlayerData* playerData = game->GetPlayerData();

	if (playerData->GetSubWeaponType() != currentSubWeaponType)
	{
		string type = playerData->GetSubWeaponType();

		if (subWeapon)
		{
			SAFE_DELETE(subWeapon);
		}

		currentSubWeaponType = type;
		subWeapon = CObjectFactory::Construct(type);
	}

	if (subWeapon)
	{
		float itemLeft, itemTop, itemRight, itemBottom;
		subWeapon->GetBoundingBox(itemLeft, itemTop, itemRight, itemBottom);

		float itemWidth = itemRight - itemLeft;
		float itemHeight = itemBottom - itemTop;

		float itemX = x + 304 + (44 - itemWidth) / 2;
		float itemY = y + 33 + (30 - itemHeight) / 2;

		subWeapon->SetPosition(itemX, itemY);
	}
}

void CBlackboard::UpdateWhipPowerType()
{
	CPlayerData* playerData = game->GetPlayerData();

	if (playerData->GetPower() != currentWhipPower)
	{
		if (whipPower)
		{
			SAFE_DELETE(whipPower);
		}

		currentWhipPower = playerData->GetPower();

		switch (game->GetPlayerData()->GetPower())
		{
		case DOUBLE_POWER:
			whipPower = new CBigDoubleShot(true);
			break;

		case TRIPLE_POWER:
			whipPower = new CBigTripleShot(true);
			break;
		}
	}

	if (whipPower)
	{
		float whipPowerX = ceil(x) + 438;
		float whipPowerY = ceil(y) + 30;

		whipPower->SetPosition(whipPowerX, whipPowerY);
	}
}

CBlackboard::~CBlackboard()
{
	SAFE_DELETE(scoreLabel);
	SAFE_DELETE(timeLabel);
	SAFE_DELETE(heartsLabel);
	SAFE_DELETE(livesLabel);
	SAFE_DELETE(simonHealthBar);
	SAFE_DELETE(enemyHealthBar);
}