#include "Animation.h"
#include "SpriteManager.h"
#include "Sprite.h"

CAnimation::CAnimation(int defaultTime)
{
	this->defaultTime = defaultTime;
	this->lastFrameTime = -1;
	this->currentFrame = -1;
}

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;

	if (time == 0)
	{
		t = this->defaultTime;
	}

	CSprite* sprite = CSpriteManager::GetInstance()->Get(spriteId);
	CAnimationFrame* frame = new CAnimationFrame(sprite, t);

	frames.push_back(frame);
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

			if (currentFrame == frames.size()) {
				currentFrame = 0;
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}
