#pragma once

#include "Scene.h"
#include "../Models/Characters/Players/Simon.h"
#include "../Game.h"
#include "../Models/Misc/Blackboard.h"
#include "../Models/Items/MoneyBag.h"
#include "../Models/Items/EasterEgg.h"
#include "../Models/Items/Dagger.h"
#include "../Models/Items/BigHeart.h"
#include "../Models/Items/MorningStar.h"
#include "../Models/Misc/BigCandle.h"
#include "../Models/Misc/Ground.h"
#include "../Models/Misc/Door.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Animations/AnimationSets.h"

class CPlayScene : public CScene
{
public:
	CPlayScene(string id, string filePath, string stage = "", string nextSceneId = "");

	virtual bool Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CSimon* GetPlayer();

protected:
	CSimon* player;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> coObjects;
	vector<CEffect*> effects;

	CGame* game;
	CBlackboard* blackboard;
	CTileMap* tileMap;

	void ParseTileMap(TiXmlElement* element);
	void ParseTextures(TiXmlElement* element);
	void ParseSprites(TiXmlElement* element);
	void ParseAnimations(TiXmlElement* element);
	void ParseAnimationSets(TiXmlElement* element);
	void ParseObjects(TiXmlElement* element);

	CGameObject* FindObject(string id);
};

class CPlaySceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	CPlaySceneKeyHandler(CScene* s) : CSceneKeyHandler(s) {};
};
