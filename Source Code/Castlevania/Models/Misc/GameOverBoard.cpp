#include "GameOverBoard.h"
#include "../../Scenes/PlayScene.h"

CGameOverBoard::CGameOverBoard()
{
	SetAnimationSet("transparency");

	gameOverLabel = new CLabel("GAME OVER", 180, 200, 17);
	continueLabel = new CLabel("CONTINUE", 196, 270, 17);
	endLabel = new CLabel("END", 196, 320, 17);

	cursor = new CCursor();

	index = CONTINUE_OPTION;
}

void CGameOverBoard::Update()
{
	CCamera* camera = CGame::GetInstance()->GetCamera();

	switch (index)
	{
	case CONTINUE_OPTION:
		cursor->SetPosition(camera->GetLeft() + 150, 268);
		break;

	case END_OPTION:
		cursor->SetPosition(camera->GetLeft() + 150, 318);
		break;
	}
}

void CGameOverBoard::Render()
{
	animationSet->at(0)->Render(x, y);

	gameOverLabel->Render();
	continueLabel->Render();
	endLabel->Render();

	cursor->Render();
}

void CGameOverBoard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CGameOverBoard::Up()
{
	CGame::GetInstance()->GetSoundManager()->Play("selecting");

	if (index == END_OPTION)
	{
		index = CONTINUE_OPTION;
	}
}

void CGameOverBoard::Down()
{
	CGame::GetInstance()->GetSoundManager()->Play("selecting");

	if (index == CONTINUE_OPTION)
	{
		index = END_OPTION;
	}
}

void CGameOverBoard::Select()
{
	CGame* game = CGame::GetInstance();
	CSceneManager* sceneManager = game->GetSceneManager();

	dynamic_cast<CPlayScene*>(sceneManager->GetCurrentScene())->HideGameOverBoard();

	switch (index)
	{
	case CONTINUE_OPTION:
		if (game->GetSceneManager()->GetCurrentScene()->GetRequiredSceneId() != "")
		{
			game->GetSceneManager()->SwitchSceneByIndex(game->GetSceneManager()->GetCurrentSceneIndex() - 1);
		}
		else
		{
			game->GetSceneManager()->SwitchSceneByIndex(game->GetSceneManager()->GetCurrentSceneIndex());
		}

		//game->GetSceneManager()->SwitchSceneByIndex(game->GetSceneManager()->GetCurrentSceneIndex());
		game->GetTimer()->SetTime(DEFAULT_GAME_TIME);
		game->GetPlayerData()->Reset();
		break;

	case END_OPTION:
		game->GetSceneManager()->SwitchSceneByIndex(INTRO_SCENE);
		break;
	}
}
