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
#include "../Models/Misc/PauseBadge.h"
#include "../Libraries/TinyXML/tinyxml.h"
#include "../Animations/AnimationSets.h"

class CPlayScene : public CScene
{
public:
	CPlayScene(string id, string filePath, string stage = "", string previousSceneId = "", string nextSceneId = "", string requiredSceneId = "");

	virtual bool Load();
	virtual bool Reload();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CSimon* GetPlayer();

protected:
	CSimon* player;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> createdObjects;
	vector<LPGAMEOBJECT> coObjects;
	vector<CUnit*> units;
	vector<LPGAMEOBJECT> grounds;

	CGame* game;
	CBlackboard* blackboard;
	CTileMap* tileMap;
	CPauseBadge* pauseBadge;

	void ParseTileMap(TiXmlElement* element);
	void ParseTextures(TiXmlElement* element);
	void ParseSprites(TiXmlElement* element);
	void ParseAnimations(TiXmlElement* element);
	void ParseAnimationSets(TiXmlElement* element);
	void ParseObjects(TiXmlElement* element, bool reloaded = false);

	bool LoadResources(bool reloaded = false);

	CGameObject* FindObject(string id);
};

class CPlaySceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);

	void OnHoldLeftKey();
	void OnHoldRightKey();

	CPlaySceneKeyHandler(CScene* s) : CSceneKeyHandler(s) {};
};
