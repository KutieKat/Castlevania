#include "Blackboard.h"
#include "../../Utilities/Debug.h"
#include "../Items/Dagger.h"
#include "../../../Utilities/SafeDelete.h"
#include "../../Game.h"

CBlackboard::CBlackboard(CSimon* simon)
{
	this->simon = simon;

	string score = padZero(this->simon->GetScore(), 6);
	string remainingTime = padZero(CGame::GetInstance()->GetTimer()->GetRemainingTime(), 4);
	string hearts = padZero(this->simon->GetHearts(), 2);
	string lives = padZero(this->simon->GetLives(), 2);

	this->scoreLabel = new CLabel(score, 103, 15);
	this->timeLabel = new CLabel(remainingTime, 288, 15);
	this->stageLabel = new CLabel("01", 460, 15);

	this->heartsLabel = new CLabel(hearts, 394, 34, 18);
	this->livesLabel = new CLabel(lives, 394, 51, 18);

	this->simonHealthBar = new CHealthBar(HealthType::Player, HEALTH_BAR_MAX_VOLUMES, this->simon->getHealthVolumes());
	this->enemyHealthBar = new CHealthBar(HealthType::Enemy, HEALTH_BAR_MAX_VOLUMES, HEALTH_BAR_MAX_VOLUMES);

	this->subWeapon = nullptr;
}

void CBlackboard::AddAnimation(string aniId)
{
	CAnimation* ani = CAnimationManager::GetInstance()->Get(aniId);
	animations.push_back(ani);
}

void CBlackboard::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CBlackboard::GetPosition(float & x, float & y)
{
	x = this->x;
	y = this->y;
}

void CBlackboard::Update(CTileMap* tileMap)
{
	if (this->simon->x < SCREEN_WIDTH / 2)
	{
		this->x = 0;
	}
	else if (this->simon->x + SCREEN_WIDTH / 2 >= tileMap->GetWidth())
	{
		this->x = tileMap->GetWidth() - SCREEN_WIDTH;
	}
	else
	{
		this->x = this->simon->x - SCREEN_WIDTH / 2;
	}

	string score = padZero(this->simon->GetScore(), 6);
	string remainingTime = padZero(CGame::GetInstance()->GetTimer()->GetRemainingTime(), 4);
	string hearts = padZero(this->simon->GetHearts(), 2);

	this->scoreLabel->SetText(score);
	this->timeLabel->SetText(remainingTime);
	this->heartsLabel->SetText(hearts);

	this->simonHealthBar->SetValue(this->simon->getHealthVolumes());
	this->simonHealthBar->SetPosition(this->x + 110, this->y + 32);

	this->enemyHealthBar->SetPosition(this->x + 110, this->y + 50);

	if (this->simon->GetSubWeaponType() == "dagger" && this->subWeapon == nullptr)
	{
		this->subWeapon = new CDagger();
		this->subWeapon->AddAnimation("dagger_right");
		this->subWeapon->SetVisibility(Visibility::Visible);
	}

	if (this->subWeapon)
	{
		float itemLeft, itemTop, itemRight, itemBottom;
		this->subWeapon->GetBoundingBox(itemLeft, itemTop, itemRight, itemBottom);

		float itemWidth = itemRight - itemLeft;
		float itemHeight = itemBottom - itemTop;

		float itemX = this->x + 304 + (44 - itemWidth) / 2;
		float itemY = this->y + 33 + (30 - itemHeight) / 2;

		this->subWeapon->SetPosition(itemX, itemY);
	}
}

void CBlackboard::Render()
{
	animations[0]->Render(x, y);

	RenderLabels();
	RenderHealthBars();

	if (this->subWeapon)
	{
		this->subWeapon->Render();
	}
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

void CBlackboard::RenderLabels()
{
	this->scoreLabel->Render();
	this->timeLabel->Render();
	this->stageLabel->Render();
	this->heartsLabel->Render();
	this->livesLabel->Render();
}

void CBlackboard::RenderHealthBars()
{
	this->simonHealthBar->Render();
	this->enemyHealthBar->Render();
}

CBlackboard::~CBlackboard()
{
	SAFE_DELETE(this->simon);
	SAFE_DELETE(this->scoreLabel);
	SAFE_DELETE(this->timeLabel);
	SAFE_DELETE(this->heartsLabel);
	SAFE_DELETE(this->livesLabel);
	SAFE_DELETE(this->simonHealthBar);
	SAFE_DELETE(this->enemyHealthBar);
	SAFE_DELETE(this->subWeapon);
}