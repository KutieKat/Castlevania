#include "PlayScene.h"

#include "../Utilities/Debug.h"
#include "../Utilities/SafeDelete.h"

void CPlayScene::ParseTileMap(TiXmlElement* element)
{
	tileMap = new CTileMap(element->Attribute("mapPath"), element->Attribute("tilesetPath"));
}

void CPlayScene::ParseTextures(TiXmlElement* element)
{
	TiXmlElement* texture = nullptr;

	for (texture = element->FirstChildElement(); texture != nullptr; texture = texture->NextSiblingElement())
	{
		int r, g, b;

		string id = texture->Attribute("id");

		string path = texture->Attribute("path");
		wstring texturePath(path.begin(), path.end());

		texture->QueryIntAttribute("r", &r);
		texture->QueryIntAttribute("g", &g);
		texture->QueryIntAttribute("b", &b);

		CTextureManager::GetInstance()->Add(id, texturePath.c_str(), CColor::FromRgb(r, g, b));
	}
}

void CPlayScene::ParseSprites(TiXmlElement* element)
{
	TiXmlElement* sprite = nullptr;

	for (sprite = element->FirstChildElement(); sprite != nullptr; sprite = sprite->NextSiblingElement())
	{
		int left, top, right, bottom;

		string id = sprite->Attribute("id");

		sprite->QueryIntAttribute("left", &left);
		sprite->QueryIntAttribute("top", &top);
		sprite->QueryIntAttribute("right", &right);
		sprite->QueryIntAttribute("bottom", &bottom);

		LPDIRECT3DTEXTURE9 texture = CTextureManager::GetInstance()->Get(sprite->Attribute("textureId"));

		CSpriteManager::GetInstance()->Add(id, left, top, right, bottom, texture);
	}
}

void CPlayScene::ParseAnimations(TiXmlElement* element)
{
	TiXmlElement* animation = nullptr;
	TiXmlElement* frame = nullptr;
	CAnimation* ani = nullptr;

	for (animation = element->FirstChildElement(); animation != nullptr; animation = animation->NextSiblingElement())
	{
		string id = animation->Attribute("id");

		ani = new CAnimation(100);

		for (frame = animation->FirstChildElement(); frame != nullptr; frame = frame->NextSiblingElement())
		{
			int duration;

			string spriteId = frame->Attribute("spriteId");
			frame->QueryIntAttribute("duration", &duration);

			ani->Add(spriteId, duration);
		}

		CAnimationManager::GetInstance()->Add(id, ani);
	}
}

void CPlayScene::ParseAnimationSets(TiXmlElement* element)
{
	TiXmlElement* animationSet = nullptr;
	TiXmlElement* animationSetItem = nullptr;

	for (animationSet = element->FirstChildElement(); animationSet != nullptr; animationSet = animationSet->NextSiblingElement())
	{
		string id = animationSet->Attribute("id");
		CAnimationSet* set = new CAnimationSet();

		for (animationSetItem = animationSet->FirstChildElement(); animationSetItem != nullptr; animationSetItem = animationSetItem->NextSiblingElement())
		{
			string animationId = animationSetItem->Attribute("animationId");

			set->emplace_back(CAnimationManager::GetInstance()->Get(animationId));
		}

		CAnimationSets::GetInstance()->Add(id, set);
	}
}

void CPlayScene::ParseObjects(TiXmlElement* element)
{
	TiXmlElement* object = nullptr;

	for (object = element->FirstChildElement(); object != nullptr; object = object->NextSiblingElement())
	{
		int x, y;
		object->QueryIntAttribute("x", &x);
		object->QueryIntAttribute("y", &y);

		string id = object->Attribute("id");
		string type = object->Attribute("type");
		string animationSetId = object->Attribute("animationSetId");
		string hiddenItemId, endingEffect;

		CAnimationSet* animationSet = CAnimationSets::GetInstance()->Get(animationSetId);

		if (type == "brick")
		{
			CBrick* item = new CBrick();
			item->SetPosition(x, y);

			objects[id] = item;
		}
		
		if (type == "big_heart")
		{
			CBigHeart* item = new CBigHeart();
			item->SetPosition(x, y);
			item->SetVisibility(Visibility::Hidden);

			objects[id] = item;
		}

		if (type == "morning_star")
		{
			CMorningStar* item = new CMorningStar();
			item->SetPosition(x, y);
			item->SetVisibility(Visibility::Hidden);

			objects[id] = item;
		}

		if (type == "dagger")
		{
			CDagger* item = new CDagger();
			item->SetPosition(x, y);
			item->SetVisibility(Visibility::Hidden);

			objects[id] = item;
		}

		if (type == "big_candle")
		{
			endingEffect = object->Attribute("endingEffect");
			hiddenItemId = object->Attribute("hiddenItemId");

			CEffect* effect = new CEffect(endingEffect);
			effects.emplace_back(effect);

			CBigCandle* item = new CBigCandle();
			item->SetPosition(x, y);
			item->SetHiddenItem(dynamic_cast<CItem*>(objects[hiddenItemId]));
			item->SetEndingEffect(effect);

			objects[id] = item;
		}

		if (type == "money_bag")
		{
			int score;

			object->QueryIntAttribute("score", &score);
			endingEffect = object->Attribute("endingEffect");

			CEffect* effect = new CEffect(endingEffect);
			effects.emplace_back(effect);

			CMoneyBag* item = new CMoneyBag();
			item->SetEndingEffect(effect);
			item->SetScore(1000);
			item->SetPosition(x, y);

			objects[id] = item;
		}

		if (type == "easter_egg")
		{
			hiddenItemId = object->Attribute("hiddenItemId");

			CEasterEgg* item = new CEasterEgg();
			item->SetPosition(x, y);
			item->SetHiddenItem(dynamic_cast<CItem*>(objects[hiddenItemId]));

			objects[id] = item;
		}

		if (type == "simon")
		{
			player = new CSimon();
			player->SetPosition(x, y);

			objects[id] = player;
		}

		if (type == "door_wall")
		{
			CDoorWall* item = new CDoorWall();
			item->SetPosition(x, y);
			item->SetVisibility(Visibility::Hidden);

			objects[id] = item;
		}

		if (type == "door")
		{
			hiddenItemId = object->Attribute("hiddenItemId");

			CDoor* item = new CDoor();
			item->SetPosition(x, y);
			item->SetDoorWall(dynamic_cast<CDoorWall*>(objects[hiddenItemId]));

			objects[id] = item;
		}
	}
}

CPlayScene::CPlayScene(string id, string filePath, string stage, string nextSceneId) : CScene(id, filePath, stage, nextSceneId)
{
	game = CGame::GetInstance();
	keyHandler = new CPlaySceneKeyHandler(this);
}

bool CPlayScene::Load()
{
	TiXmlDocument doc(this->filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Error(doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* tileMap1 = root->FirstChildElement();
	TiXmlElement* textures = tileMap1->NextSiblingElement();
	TiXmlElement* sprites = textures->NextSiblingElement();
	TiXmlElement* animations = sprites->NextSiblingElement();
	TiXmlElement* animationSets = animations->NextSiblingElement();
	TiXmlElement* objects = animationSets->NextSiblingElement();

	ParseTileMap(tileMap1);
	ParseTextures(textures);
	ParseSprites(sprites);
	ParseAnimations(animations);
	ParseAnimationSets(animationSets);
	ParseObjects(objects);

	blackboard = new CBlackboard();
	blackboard->SetPlayer(player);
	blackboard->SetTileMap(tileMap);

	CGame::GetInstance()->GetTimer()->Start();

	return true;
}

void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;

	for (auto x : objects)
	{
		if (x.second->GetVisibility() == Visibility::Visible)
		{
			if (dynamic_cast<CSimon*>(x.second))
			{
				continue;
			}
			else
			{
				coObjects.emplace_back(x.second);
			}
		}
	}

	for (auto x : objects)
	{
		if (x.second->GetVisibility() == Visibility::Visible)
		{
			x.second->Update(dt, &coObjects);
		}
	}

	// Update timer
	game->GetTimer()->Tick();

	// Update camera to follow the player
	float cx, cy;

	if (player) player->GetPosition(cx, cy);

	float currentPlayerX;
	float currentPlayerY;

	if (player) player->GetPosition(currentPlayerX, currentPlayerY);

	if (tileMap)
	{
		if (currentPlayerX < SCREEN_WIDTH / 2) {
			cx = 0.0f;

			if (currentPlayerX <= 0) {
				currentPlayerX = 0.0f;
				player->SetPosition(currentPlayerX, currentPlayerY);
			}
		}
		else if (currentPlayerX + SCREEN_WIDTH / 2 >= tileMap->GetWidth()) {
			float left, top, right, bottom;

			player->GetBoundingBox(left, top, right, bottom);
			cx = tileMap->GetWidth() - SCREEN_WIDTH;

			float playerBoundingBoxWidth = right - left;

			if (currentPlayerX >= tileMap->GetWidth() - playerBoundingBoxWidth) {
				currentPlayerX = tileMap->GetWidth() - playerBoundingBoxWidth;
				player->SetPosition(currentPlayerX, currentPlayerY);
			}
		}
		else {
			cx -= SCREEN_WIDTH / 2;
		}

		cy -= SCREEN_HEIGHT / 2;
	}

	// Update blackboard position
	if (blackboard)
	{
		blackboard->Update();
	}

	// Update camera position
	game->GetCamera()->SetPosition(cx, 0.0f);
}

void CPlayScene::Render()
{
	// TileMap
	if (tileMap)
	{
		tileMap->Render(game->GetCamera());
	}

	// Effects
	if (effects.size() > 0)
	{
		for (int i = 0; i < effects.size(); i++)
		{
			if (effects[i]->GetStartTime() != -1)
			{
				effects[i]->Render();
			}
		}
	}

	for (auto x : objects)
	{
		if (x.second->GetVisibility() == Visibility::Visible)
		{
			x.second->Render();
		}
	}

	// Blackboard
	if (blackboard)
	{
		blackboard->Render();
	}
}

void CPlayScene::Unload()
{	
	// Unload blackboard :)
	// Unload effects :)
	// Unload tilemap :)
	// Unload objects :(
	// Unload player :(

	SAFE_DELETE(blackboard);
	SAFE_DELETE(tileMap);

	for (int i = 0; i < effects.size(); i++)
	{
		SAFE_DELETE(effects[i]);
	}

	effects.clear();
}

CSimon* CPlayScene::GetPlayer()
{
	return this->player;
}

void CPlaySceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();

	if (simon != nullptr)
	{
		if (simon->Up() && simon->TouchingGround() == false && simon->animationSet->at(simon->GetAnimationToRender())->Over() == false)
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_JUMP && simon->TouchingGround() == false)
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_STAND_AND_THROW && simon->animationSet->at(simon->GetAnimationToRender())->Over() == false)
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && simon->animationSet->at(simon->GetAnimationToRender())->Over() == false)
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK && simon->animationSet->at(simon->GetAnimationToRender())->Over() == false)
		{
			return;
		}

		if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
		{
			simon->SetState(SIMON_STATE_SIT);
		}

		if (game->GetInputManager()->IsKeyDown(DIK_UP))
		{
			simon->SetUp(true);
		}

		if (game->GetInputManager()->IsKeyDown(DIK_RIGHT))
		{
			simon->SetDirection(Direction::Right);

			if (!simon->Sitting())
			{
				simon->SetState(SIMON_STATE_WALK);
			}
		}
		else if (game->GetInputManager()->IsKeyDown(DIK_LEFT))
		{
			simon->SetDirection(Direction::Left);

			if (!simon->Sitting())
			{
				simon->SetState(SIMON_STATE_WALK);
			}
		}
		else if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
		{
			simon->SetState(SIMON_STATE_SIT);
		}
		else
		{
			if (game->GetTimer()->GetRemainingTime() > 0)
			{
				simon->SetState(SIMON_STATE_IDLE);
			}
			else
			{
				simon->SetState(SIMON_STATE_DIE);		
			}
		}
	}
}

void CPlaySceneKeyHandler::OnKeyDown(int keyCode)
{
	CGame* game = CGame::GetInstance();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();

	if (simon != nullptr)
	{
		switch (keyCode)
		{
		case DIK_R:
			game->SwitchScene(game->GetCurrentScene()->GetNextSceneId());
			break;

		case DIK_Z:
			if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && simon->animationSet->at(simon->GetAnimationToRender())->Over() == false)
			{
				return;
			}

			if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK || (simon->Sitting() == true && simon->GetState() == SIMON_STATE_SIT_AND_ATTACK))
			{
				return;
			}

			if (game->GetSubWeaponType() != "" && game->GetHearts() > 0 && simon->Up())
			{
				simon->SetState(SIMON_STATE_STAND_AND_THROW);
			}

			if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_WALK || simon->GetState() == SIMON_STATE_JUMP)
			{
				simon->SetState(SIMON_STATE_STAND_AND_ATTACK);
			}
			else if (simon->GetState() == SIMON_STATE_SIT)
			{
				simon->SetState(SIMON_STATE_SIT_AND_ATTACK);
			}

			break;

		case DIK_X:
			if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && simon->TouchingGround() == false && simon->animationSet->at(simon->GetAnimationToRender())->Over() == false)
			{
				return;
			}

			if (simon->Sitting())
			{
				return;
			}

			simon->SetState(SIMON_STATE_JUMP);

			break;

		case DIK_UP:
			simon->SetUp(true);
			break;

		default:
			break;
		}
	}
}

void CPlaySceneKeyHandler::OnKeyUp(int keyCode)
{
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();

	if (simon != nullptr)
	{
		switch (keyCode)
		{
		case DIK_UP:
			simon->SetUp(false);
			break;

		case DIK_DOWN:
			simon->SetState(SIMON_STATE_IDLE);
			break;
		}
	}

}
