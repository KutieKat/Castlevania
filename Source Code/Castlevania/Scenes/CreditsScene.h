#pragma once

#include <string>

#include "Scene.h"
#include "../Game.h"
#include "../Models/Misc/Background.h"
#include "../Utilities/Color.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Animations/AnimationSets.h"
using namespace std;

class CCreditsScene : public CScene
{
public:
	CCreditsScene(string id, string filePath, string stage = "", string previousSceneId = "", string nextSceneId = "", string requiredSceneId = "");

	virtual bool Load();
	virtual bool Reload();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SetSwitchSceneTime(DWORD time);

protected:
	vector<LPGAMEOBJECT> objects;

	CGame* game;

	void ParseSounds(TiXmlElement* element);
	void ParseTextures(TiXmlElement* element);
	void ParseSprites(TiXmlElement* element);
	void ParseAnimations(TiXmlElement* element);
	void ParseAnimationSets(TiXmlElement* element);
	void ParseObjects(TiXmlElement* element);
};

class CCreditsSceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	CCreditsSceneKeyHandler(CScene* s) : CSceneKeyHandler(s) {};
};
