#include "PlayScene.h"
#include "../Models/ObjectFactory.h"
#include "../Models/Misc/TopStair.h"
#include "../Models/Misc/BottomStair.h"
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
		int x, y, leftBound, topBound, rightBound, bottomBound, value = 0;
		bool visibility = true;

		object->QueryIntAttribute("x", &x);
		object->QueryIntAttribute("y", &y);

		string id = object->Attribute("id");
		string type = object->Attribute("type");
		string animationSetId = object->Attribute("animationSetId");
		string hiddenItemId, endingEffect;

		CAnimationSet* animationSet = CAnimationSets::GetInstance()->Get(animationSetId);
		CGameObject* gameObject = nullptr;

		if (type == "simon")
		{
			string state, stairDirectionX, stairDirectionY;
			string reloadState, reloadStairDirectionX, reloadStairDirectionY;

			player = new CSimon();
			player->SetId(id);

			if (!reloading)
			{
				if (object->Attribute("state"))
				{
					state = object->Attribute("state");
				}

				if (object->Attribute("stairDirectionX"))
				{
					stairDirectionX = object->Attribute("stairDirectionX");
				}

				if (object->Attribute("stairDirectionY"))
				{
					stairDirectionY = object->Attribute("stairDirectionY");
				}

				player->SetPosition(x, y);
				player->stairDirectionX = stairDirectionX == "right" ? Direction::Right : Direction::Left;
				player->stairDirectionY = stairDirectionY == "up" ? Direction::Up : Direction::Down;
			}
			else
			{
				int reloadX, reloadY;

				object->QueryIntAttribute("reloadX", &reloadX);
				object->QueryIntAttribute("reloadY", &reloadY);

				if (object->Attribute("reloadState"))
				{
					reloadState = object->Attribute("reloadState");
				}

				if (object->Attribute("reloadStairDirectionX"))
				{
					reloadStairDirectionX = object->Attribute("reloadStairDirectionX");
				}

				if (object->Attribute("reloadStairDirectionY"))
				{
					reloadStairDirectionY = object->Attribute("reloadStairDirectionY");
				}

				player->SetPosition(reloadX, reloadY);
				player->stairDirectionX = reloadStairDirectionX == "right" ? Direction::Right : Direction::Left;
				player->stairDirectionY = reloadStairDirectionY == "up" ? Direction::Up : Direction::Down;
			}

			if (state == "on_stair" || (reloading && reloadState == "on_stair"))
			{
				player->onStair = true;
				player->SetState(SIMON_STATE_IDLE_ON_STAIR);
			}

			objects.emplace_back(player);
		}
		else if (type == "brick")
		{
			bool isGround = false;
			object->QueryBoolAttribute("isGround", &isGround);

			CBrick* brick = new CBrick();
			brick->SetId(id);
			brick->SetPosition(x, y);
			brick->isGround = isGround;

			objects.emplace_back(brick);
		}
		else if (type == "breakable_brick")
		{
			bool isGround = false;
			object->QueryBoolAttribute("isGround", &isGround);

			CBreakableBrick* brick = new CBreakableBrick();
			brick->SetId(id);
			brick->SetPosition(x, y);
			brick->isGround = isGround;

			if (object->Attribute("hiddenItemId"))
			{
				brick->SetHiddenItem(FindObject(object->Attribute("hiddenItemId")));
			}

			if (object->Attribute("endingEffect"))
			{
				CGameObject* effect = CObjectFactory::Construct(object->Attribute("endingEffect"), x, y);
				objects.emplace_back(effect);

				brick->SetEndingEffect(effect);
			}

			objects.emplace_back(brick);
		}
		else if (type == "top_stair")
		{
			string directionX = object->Attribute("directionX");
			string directionY = object->Attribute("directionY");

			CTopStair* stair = new CTopStair();
			stair->SetId(id);
			stair->SetPosition(x, y);
			stair->directionX = directionX == "right" ? Direction::Right : Direction::Left;
			stair->directionY = directionY == "up" ? Direction::Up : Direction::Down;

			objects.emplace_back(stair);
		}
		else if (type == "bottom_stair")
		{
			string directionX = object->Attribute("directionX");
			string directionY = object->Attribute("directionY");

			CBottomStair* stair = new CBottomStair();
			stair->SetId(id);
			stair->SetPosition(x, y);
			stair->directionX = directionX == "right" ? Direction::Right : Direction::Left;
			stair->directionY = directionY == "up" ? Direction::Up : Direction::Down;
			
			objects.emplace_back(stair);
		}
		else if (type == "next_scene")
		{
			bool playerMustBeOnStair = false;
			string sceneDirection = object->Attribute("sceneDirection");

			object->QueryBoolAttribute("playerMustBeOnStair", &playerMustBeOnStair);

			CNextScene* scene = new CNextScene();
			scene->SetId(id);
			scene->SetPosition(x, y);
			scene->playerMustBeOnStair = playerMustBeOnStair;
			scene->sceneDirection = sceneDirection == "right" ? Direction::Right : Direction::Left;

			objects.emplace_back(scene);
		}
		else if (type == "previous_scene")
		{
			bool playerMustBeOnStair = false;
			string sceneDirection = object->Attribute("sceneDirection");

			object->QueryBoolAttribute("playerMustBeOnStair", &playerMustBeOnStair);

			CPreviousScene* scene = new CPreviousScene();
			scene->SetId(id);
			scene->SetPosition(x, y);
			scene->playerMustBeOnStair = playerMustBeOnStair;
			scene->sceneDirection = sceneDirection == "right" ? Direction::Right : Direction::Left;

			objects.emplace_back(scene);
		}
		else
		{
			gameObject = CObjectFactory::Construct(type, x, y);
		}

		if (gameObject)
		{
			gameObject->SetId(id);
			gameObject->SetPosition(x, y);

			object->QueryBoolAttribute("visible", &visibility);
			gameObject->SetVisibility(visibility ? Visibility::Visible : Visibility::Hidden);

			if (object->Attribute("hiddenItemId"))
			{
				gameObject->SetHiddenItem(FindObject(object->Attribute("hiddenItemId")));
			}

			if (object->Attribute("endingEffect"))
			{
				CGameObject* effect = CObjectFactory::Construct(object->Attribute("endingEffect"));
				effect->SetPosition(x, y);
				objects.emplace_back(effect);

				gameObject->SetEndingEffect(effect);
			}

			object->QueryIntAttribute("value", &value);
			gameObject->SetValue(value);

			object->QueryIntAttribute("leftBound", &leftBound);
			gameObject->SetLeftBound(leftBound);

			object->QueryIntAttribute("topBound", &topBound);
			gameObject->SetTopBound(topBound);

			object->QueryIntAttribute("rightBound", &rightBound);
			gameObject->SetRightBound(rightBound);

			object->QueryIntAttribute("bottomBound", &bottomBound);
			gameObject->SetBottomBound(bottomBound);

			objects.emplace_back(gameObject);
		}
	}
}

CGameObject* CPlayScene::FindObject(string id)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetId() == id)
		{
			return objects[i];
		}
	}
}

CPlayScene::CPlayScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId) : CScene(id, filePath, stage, previousSceneId, nextSceneId)
{
	game = CGame::GetInstance();
	reloading = false;
	keyHandler = new CPlaySceneKeyHandler(this);
}

bool CPlayScene::Load()
{
	TiXmlDocument doc(filePath.c_str());

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

bool CPlayScene::Reload()
{
	reloading = true;

	return Load();
}

void CPlayScene::Update(DWORD dt)
{
	coObjects.clear();

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetVisibility() == Visibility::Visible && !objects[i]->isEffect)
		{
			if (dynamic_cast<CSimon*>(objects[i]))
			{
				continue;
			}
			else
			{
				coObjects.emplace_back(objects[i]);
			}
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetVisibility() == Visibility::Visible)
		{
			objects[i]->Update(dt, &coObjects);
		}
	}

	// Update timer
	game->GetTimer()->Tick();

	// Update camera to follow the player
	float cx = 0, cy = 0;

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
			cx = tileMap->GetWidth() > SCREEN_WIDTH + 50 ? cx = tileMap->GetWidth() - SCREEN_WIDTH : 0;

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

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetVisibility() == Visibility::Visible)
		{
			objects[i]->Render();
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
	SAFE_DELETE(blackboard);
	SAFE_DELETE(tileMap);

	for (int i = 0; i < objects.size(); i++)
	{
		SAFE_DELETE(objects[i]);
	}

	objects.clear();

	coObjects.clear();

	player = nullptr;
}

CSimon* CPlayScene::GetPlayer()
{
	return this->player;
}

void CPlaySceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();
	CPlayerData* playerData = game->GetPlayerData();

	if (simon != nullptr)
	{
		if (simon->GetState() == SIMON_STATE_STAND_ON_STAIR_AND_THROW && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_DEFLECT && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
		{
			return;
		}

		if ((simon->GetState() == SIMON_STATE_WALK_UPSTAIR_AND_ATTACK || simon->GetState() == SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK) && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
		{
			return;
		}

		if (simon->up && !simon->touchingGround && simon->animationSet->at(simon->GetAnimationToRender())->Over())
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_JUMP && !simon->touchingGround)
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_STAND_AND_THROW && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
		{
			return;
		}

		if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
		{
			return;
		}

		if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
		{
			if (!simon->onStair)
			{
				simon->SetState(SIMON_STATE_SIT);
			}
		}

		if (game->GetInputManager()->IsKeyDown(DIK_UP))
		{
			simon->up = true;
		}

		if (game->GetInputManager()->IsKeyDown(DIK_UP))
		{
			if (simon->onStair)
			{
				simon->SetDirectionX(Direction::Right);
				simon->SetState(SIMON_STATE_WALK_UPSTAIR);
			}
			else
			{
				simon->SetState(SIMON_STATE_IDLE);
			}
		}
		if (game->GetInputManager()->IsKeyDown(DIK_RIGHT))
		{
			simon->SetDirectionX(Direction::Right);

			if (!simon->sitting)
			{
				simon->SetState(SIMON_STATE_WALK);
			}

			if (simon->onStair)
			{
				simon->SetState(SIMON_STATE_WALK_UPSTAIR);
			}
		}
		else if (game->GetInputManager()->IsKeyDown(DIK_LEFT))
		{
			simon->SetDirectionX(Direction::Left);

			if (!simon->sitting)
			{
				simon->SetState(SIMON_STATE_WALK);
			}

			if (simon->onStair)
			{
				simon->SetState(SIMON_STATE_WALK_DOWNSTAIR);
			}
		}
		else if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
		{
			if (!simon->onStair)
			{
				simon->SetState(SIMON_STATE_SIT);
			}
			else
			{
				simon->SetDirectionX(Direction::Left);
				simon->SetState(SIMON_STATE_WALK_DOWNSTAIR);
			}
		}
		else
		{
			if (game->GetTimer()->GetRemainingTime() > 0 && playerData->GetHealthVolumes() > 0)
			{
				if (simon->onStair)
				{
					simon->SetState(SIMON_STATE_IDLE_ON_STAIR);
				}
				else
				{
					simon->SetState(SIMON_STATE_IDLE);
				}
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
	CPlayerData* playerData = game->GetPlayerData();

	if (simon != nullptr)
	{
		switch (keyCode)
		{
		case DIK_Z:
			if (simon->GetState() == SIMON_STATE_WALK_UPSTAIR_AND_ATTACK && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
			{
				return;
			}

			if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
			{
				return;
			}

			if (simon->GetState() == SIMON_STATE_SIT_AND_ATTACK || (simon->sitting && simon->GetState() == SIMON_STATE_SIT_AND_ATTACK))
			{
				return;
			}

			if (simon->onStair)
			{
				if (simon->GetDirectionY() == Direction::Up)
				{
					simon->SetState(SIMON_STATE_WALK_UPSTAIR_AND_ATTACK);
				}
				else
				{
					simon->SetState(SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK);
				}

				if (playerData->GetSubWeaponType() != "" && playerData->GetHearts() > 0 && simon->up)
				{
					simon->SetState(SIMON_STATE_STAND_ON_STAIR_AND_THROW);
				}
			}
			else
			{
				if (playerData->GetSubWeaponType() != "" && playerData->GetHearts() > 0 && simon->up)
				{
					simon->SetState(SIMON_STATE_STAND_AND_THROW);
				}
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
			if (simon->onStair)
			{
				return;
			}

			if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && !simon->touchingGround && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
			{
				return;
			}

			if (simon->sitting)
			{
				return;
			}

			simon->SetState(SIMON_STATE_JUMP);

			break;

		case DIK_UP:
			simon->up = true;

			if (simon->atBottomStair && simon->GetDirectionX() == simon->stairDirectionX)
			{
				simon->onStair = true;
				simon->SetState(SIMON_STATE_WALK_UPSTAIR);
			}

			break;

		case DIK_DOWN:
			if (simon->onStair)
			{
				return;
			}

			if (simon->atTopStair && simon->GetDirectionX() == simon->stairDirectionX)
			{
				simon->onStair = true;
				simon->SetState(SIMON_STATE_WALK_DOWNSTAIR);
			}

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
			simon->up = false;
			break;

		case DIK_DOWN:
			if (simon->onStair)
			{
				simon->SetState(SIMON_STATE_IDLE_ON_STAIR);
			}
			else
			{
				simon->SetState(SIMON_STATE_IDLE);
			}

			break;
		}
	}

}
