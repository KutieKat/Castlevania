#pragma once

#include <string>

#include "Scene.h"
#include "../Models/Characters/Players/Simon.h"
#include "../Game.h"
#include "../Models/Misc/Background.h"
#include "../Utilities/Color.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Animations/AnimationSets.h"
#include "../Utilities/Label.h"
#include "../Models/Misc/IntroBat.h"
using namespace std;

class CIntroScene : public CScene
{
public:
	CIntroScene(string id, string filePath, string stage = "", string nextSceneId = "");

	virtual bool Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SetSwitchSceneTime(DWORD time);

protected:
	vector<LPGAMEOBJECT> objects;

	CGame* game;
	CLabel* startLabel;

	int interval = 0;

	void ParseTextures(TiXmlElement* element);
	void ParseSprites(TiXmlElement* element);
	void ParseAnimations(TiXmlElement* element);
	void ParseAnimationSets(TiXmlElement* element);
	void ParseObjects(TiXmlElement* element);
};

class CIntroSceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	CIntroSceneKeyHandler(CScene* s) : CSceneKeyHandler(s) {};
};
