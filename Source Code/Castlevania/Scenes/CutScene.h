#pragma once

#include <string>

#include "Scene.h"
#include "../Models/Characters/Players/Simon.h"
#include "../Game.h"
#include "../Models/Misc/Background.h"
#include "../Utilities/Color.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Animations/AnimationSets.h"
#include "../Models/Misc/Bat.h"
#include "../Models/Misc/Blackboard.h"
#include "../Models/Misc/Helicopter.h"
using namespace std;

class CCutScene : public CScene
{
public:
	CCutScene(string id, string filePath, string stage = "", string previousSceneId = "", string nextSceneId = "", string requiredSceneId = "");

	virtual bool Load();
	virtual bool Reload();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

protected:
	vector<LPGAMEOBJECT> objects;

	CSimon* player;
	CGame* game;
	CBlackboard* blackboard;

	void ParseTextures(TiXmlElement* element);
	void ParseSprites(TiXmlElement* element);
	void ParseAnimations(TiXmlElement* element);
	void ParseAnimationSets(TiXmlElement* element);
	void ParseObjects(TiXmlElement* element);
};

class CCutSceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	CCutSceneKeyHandler(CScene* s) : CSceneKeyHandler(s) {};
};
