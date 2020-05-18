#include "Animation.h"
#include "../Sprites/SpriteManager.h"
#include "../Sprites/Sprite.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Debug.h"
#include "../Utilities/SafeDelete.h"

CAnimation::CAnimation(int defaultTime)
{
	this->defaultTime = defaultTime;
	this->lastFrameTime = -1;
	this->currentFrame = -1;
	this->endTime = -1;
}

void CAnimation::Add(string spriteId, DWORD time)
{
	int t = time;

	if (time == 0)
	{
		t = defaultTime;
	}

	CSprite* sprite = CSpriteManager::GetInstance()->Get(spriteId);

	if (sprite == nullptr)
	{
		CDebug::Error("Failed to find sprite id=" + spriteId, "Animation.cpp");
	}

	CAnimationFrame* frame = new CAnimationFrame(sprite, t);

	frames.emplace_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();

		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::SetStartTime(DWORD start)
{
	DWORD animationTime = 0;

	for (int i = 0; i < frames.size(); i++)
	{
		animationTime += frames[i]->GetTime();
	}

	endTime = start + animationTime;
}

void CAnimation::Pause()
{
	DWORD now = GetTickCount();

	currentFrame = currentFrame;
	lastFrameTime = now;
}

bool CAnimation::Over()
{
	return GetTickCount() > endTime;
}

bool CAnimation::ReachLastFrame()
{
	return currentFrame == frames.size() - 1;
}

void CAnimation::Reset()
{
	lastFrameTime = -1;
	currentFrame = -1;
}

CAnimation::~CAnimation()
{
	for (int i = 0; i < frames.size(); i++)
	{
		SAFE_DELETE(frames[i]);
	}

	frames.clear();
}