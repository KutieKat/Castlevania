#include "Scene.h"
#include "../Game.h"

CScene::CScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId, string requiredSceneId)
{
	this->id = id;
	this->filePath = filePath;
	this->previousSceneId = previousSceneId;
	this->nextSceneId = nextSceneId;
	this->stage = stage;
	this->switchSceneTime = -1;
	this->softPauseTime = -1;
	this->needReloading = false;
	this->requiredSceneId = requiredSceneId;
	this->softPaused = false;
	this->hardPaused = false;
}

IKeyEventHandler* CScene::GetKeyEventHandler()
{
	return keyHandler;
}

void CScene::SoftPause(DWORD duration)
{
	softPaused = true;

	if (duration != -1)
	{
		CGame::GetInstance()->GetSoundManager()->Play("stopwatch_ticking");
		softPauseTime = GetTickCount() + CGame::GetInstance()->GetPauseDeltaTime() + duration;
	}
}

void CScene::HardPause(bool showingPauseBadge)
{
	this->hardPaused = true;
	this->showingPauseBadge = showingPauseBadge;
}

void CScene::ResumeSoftPause()
{
	CGame::GetInstance()->GetSoundManager()->Stop("stopwatch_ticking");
	softPaused = false;
}

void CScene::ResumeHardPause()
{
	hardPaused = false;
}

bool CScene::SoftPaused()
{
	return softPaused;
}

bool CScene::HardPaused()
{
	return hardPaused;
}

bool CScene::ShowingPauseBadge()
{
	return showingPauseBadge;
}

string CScene::GetId()
{
	return id;
}

string CScene::GetPreviousSceneId()
{
	return previousSceneId;
}

string CScene::GetNextSceneId()
{
	return nextSceneId;
}

string CScene::GetRequiredSceneId()
{
	return requiredSceneId;
}

string CScene::GetStage()
{
	return stage;
}

CGrid* CScene::GetGrid()
{
	return grid;
}
