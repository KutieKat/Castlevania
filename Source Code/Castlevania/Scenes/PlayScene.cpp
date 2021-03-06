﻿#include "PlayScene.h"
#include "../Models/ObjectFactory.h"
#include "../Models/Misc/TopStair.h"
#include "../Models/Misc/BottomStair.h"
#include "../Models/Misc/BiStair.h"
#include "../Models/Misc/EnemySpawnerArea.h"
#include "../Utilities/Debug.h"
#include "../Utilities/SafeDelete.h"

void CPlayScene::ParseSounds(TiXmlElement* element)
{
	CGameSoundManager* soundManager = CGameSoundManager::GetInstance();
	TiXmlElement* sound = nullptr;

	for (sound = element->FirstChildElement(); sound != nullptr; sound = sound->NextSiblingElement())
	{
		string id = sound->Attribute("id");
		string path = sound->Attribute("path");

		bool loop = false;
		sound->QueryBoolAttribute("loop", &loop);

		bool retained = false;
		sound->QueryBoolAttribute("retained", &retained);

		bool isBackground = false;
		sound->QueryBoolAttribute("isBackground", &isBackground);

		if (path != "")
		{
			soundManager->Add(id, path, loop, retained, isBackground);
		}
	}
}

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

void CPlayScene::ParseObjects(TiXmlElement* element, bool reloaded)
{
	TiXmlElement* root = element;
	int gridWidth, gridHeight, cellWidth, cellHeight;

	root->QueryIntAttribute("gridWidth", &gridWidth);
	root->QueryIntAttribute("gridHeight", &gridHeight);
	root->QueryIntAttribute("cellWidth", &cellWidth);
	root->QueryIntAttribute("cellHeight", &cellHeight);

	TiXmlElement* object = nullptr;
	CUnit* unit = nullptr;

	grid = new CGrid(gridWidth, gridHeight, cellWidth, cellHeight);

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
			string state, stairDirectionX, stairDirectionY, directionX;
			string reloadState, reloadStairDirectionX, reloadStairDirectionY, reloadDirectionX;

			player = new CSimon();
			player->SetId(id);

			if (!reloaded)
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

				if (object->Attribute("directionX"))
				{
					directionX = object->Attribute("directionX");
				}

				player->SetPosition(x, y);
				player->SetDirectionX(directionX == "left" ? Direction::Left : Direction::Right);
				player->stairDirectionX = stairDirectionX == "right" ? Direction::Right : Direction::Left;
				player->stairDirectionY = stairDirectionY == "" ? Direction::None : (stairDirectionY == "up" ? Direction::Up : Direction::Down);
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

				if (object->Attribute("reloadDirectionX"))
				{
					reloadDirectionX = object->Attribute("reloadDirectionX");
				}

				player->SetPosition(reloadX, reloadY);
				player->SetDirectionX(reloadDirectionX == "left" ? Direction::Left : Direction::Right);
				player->stairDirectionX = reloadStairDirectionX == "right" ? Direction::Right : Direction::Left;
				player->stairDirectionY = reloadStairDirectionY == "" ? Direction::None : (reloadStairDirectionY == "up" ? Direction::Up : Direction::Down);
			}

			if (state == "on_stair" || (reloaded && reloadState == "on_stair"))
			{
				player->onStair = true;
				player->SetDirectionX(player->stairDirectionX);
				player->SetDirectionY(player->stairDirectionY);
				player->SetState(SIMON_STATE_IDLE_ON_STAIR);
			}

			unit = new CUnit(grid, player, player->x, player->y);
		}
		else if (type == "brick")
		{
			bool canJumpThrough = false;
			object->QueryBoolAttribute("canJumpThrough", &canJumpThrough);

			int width, height;
			object->QueryIntAttribute("width", &width);
			object->QueryIntAttribute("height", &height);

			CBrick* brick = new CBrick();
			brick->SetId(id);
			brick->SetPosition(x, y);
			brick->SetWidth(width);
			brick->SetHeight(height);
			brick->canJumpThrough = canJumpThrough;

			grounds.emplace_back(brick);
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
				unit = new CUnit(grid, effect, x, y);

				brick->SetEndingEffect(effect);
			}

			unit = new CUnit(grid, brick, x, y);
		}
		else if (type == "easter_egg")
		{
			bool mustSit = false;
			object->QueryBoolAttribute("mustSit", &mustSit);

			CEasterEgg* easterEgg = new CEasterEgg();
			easterEgg->SetId(id);
			easterEgg->SetPosition(x, y);
			easterEgg->SetMustSit(mustSit);

			if (object->Attribute("hiddenItemId"))
			{
				easterEgg->SetHiddenItem(FindObject(object->Attribute("hiddenItemId")));
			}

			unit = new CUnit(grid, easterEgg, x, y);
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

			unit = new CUnit(grid, stair, x, y);
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
			
			unit = new CUnit(grid, stair, x, y);
		}
		else if (type == "bi_stair")
		{
			string upsideDirectionX = object->Attribute("upsideDirectionX");
			string upsideDirectionY = object->Attribute("upsideDirectionY");
			string downsideDirectionX = object->Attribute("downsideDirectionX");
			string downsideDirectionY = object->Attribute("downsideDirectionY");

			CBiStair* stair = new CBiStair();
			stair->SetId(id);
			stair->SetPosition(x, y);
			stair->upsideDirectionX = upsideDirectionX == "right" ? Direction::Right : Direction::Left;
			stair->upsideDirectionY = upsideDirectionY == "up" ? Direction::Up : Direction::Down;
			stair->downsideDirectionX = downsideDirectionX == "right" ? Direction::Right : Direction::Left;
			stair->downsideDirectionY = downsideDirectionY == "up" ? Direction::Up : Direction::Down;

			unit = new CUnit(grid, stair, x, y);
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

			unit = new CUnit(grid, scene, x, y);
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

			unit = new CUnit(grid, scene, x, y);
		}
		else if (type == "enemy_spawner_area")
		{
			string enemyType = object->Attribute("enemyType");
			string spawningPosition = object->Attribute("spawningPosition");

			CEnemySpawnerArea* area = new CEnemySpawnerArea(player);
			area->SetId(id);
			area->SetPosition(x, y);
			area->SetEnemyType(enemyType);
			area->SetSpawningPosition(spawningPosition);

			unit = new CUnit(grid, area, x, y);
		}
		else
		{
			gameObject = CObjectFactory::Construct(type, x, y, player);
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
				unit = new CUnit(grid, effect, x, y);

				gameObject->SetEndingEffect(effect);
			}

			object->QueryIntAttribute("leftBound", &leftBound);
			gameObject->SetLeftBound(leftBound);

			object->QueryIntAttribute("topBound", &topBound);
			gameObject->SetTopBound(topBound);

			object->QueryIntAttribute("rightBound", &rightBound);
			gameObject->SetRightBound(rightBound);

			object->QueryIntAttribute("bottomBound", &bottomBound);
			gameObject->SetBottomBound(bottomBound);

			createdObjects.emplace_back(gameObject);
			unit = new CUnit(grid, gameObject, x, y);
		}
	}
}

bool CPlayScene::LoadResources(bool reloaded)
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Error(doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* sounds = root->FirstChildElement();
	TiXmlElement* tileMap1 = sounds->NextSiblingElement();
	TiXmlElement* textures = tileMap1->NextSiblingElement();
	TiXmlElement* sprites = textures->NextSiblingElement();
	TiXmlElement* animations = sprites->NextSiblingElement();
	TiXmlElement* animationSets = animations->NextSiblingElement();
	TiXmlElement* objects = animationSets->NextSiblingElement();

	ParseSounds(sounds);
	ParseTileMap(tileMap1);
	ParseTextures(textures);
	ParseSprites(sprites);
	ParseAnimations(animations);
	ParseAnimationSets(animationSets);
	ParseObjects(objects, reloaded);

	blackboard = new CBlackboard();
	blackboard->SetPlayer(player);
	blackboard->SetTileMap(tileMap);

	pauseBadge = new CPauseBadge();
	gameOverBoard = new CGameOverBoard();

	CGame::GetInstance()->GetSoundManager()->PlayBackgroundSounds();

	return true;
}

CGameObject* CPlayScene::FindObject(string id)
{
	for (int i = 0; i < createdObjects.size(); i++)
	{
		if (createdObjects[i]->GetId() == id)
		{
			return createdObjects[i];
		}
	}
}

CPlayScene::CPlayScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId, string requiredSceneId) : CScene(id, filePath, stage, previousSceneId, nextSceneId, requiredSceneId)
{
	game = CGame::GetInstance();
	keyHandler = new CPlaySceneKeyHandler(this);

	showingGameOverBoard = false;
}

void CPlayScene::ShowGameOverBoard()
{
	showingGameOverBoard = true;
}

void CPlayScene::HideGameOverBoard()
{
	showingGameOverBoard = false;
}

bool CPlayScene::ShowingGameOverBoard()
{
	return showingGameOverBoard;
}

CGameOverBoard* CPlayScene::GetGameOverBoard()
{
	return gameOverBoard;
}

bool CPlayScene::Load()
{
	return LoadResources();
}

bool CPlayScene::Reload()
{
	return LoadResources(true);
}

void CPlayScene::Update(DWORD dt)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (softPauseTime != -1 && GetTickCount() > softPauseTime)
	{
		ResumeSoftPause();
		softPauseTime = -1;
	}

	if (softPaused)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CEnemy*>(objects[i]))
			{
				objects[i]->Pause();
			}
		}
	}

	if (hardPaused)
	{
		player->Pause();

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Pause();
		}
	}
	else
	{
		units.clear();
		objects.clear();
		coObjects.clear();

		if (grid)
		{
			grid->Clean();
			units = grid->Get(CGame::GetInstance()->GetCamera());
		}

		for (int i = 0; i < units.size(); i++)
		{
			CGameObject* object = units[i]->GetGameObject();
			objects.push_back(object);

			units[i]->Move(object->x, object->y);
		}

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->isEffect)
			{
				continue;
			}
			else if (dynamic_cast<CSimon*>(objects[i]))
			{
				continue;
			}
			else
			{
				coObjects.emplace_back(objects[i]);
			}
		}

		for (int i = 0; i < grounds.size(); i++)
		{
			coObjects.emplace_back(grounds[i]);
		}

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(dt, &coObjects);
		}

		// Update camera to follow the player
		float cx = 0, cy = 0;

		if (player) player->GetPosition(cx, cy);

		float currentPlayerX = 0;
		float currentPlayerY = 0;

		if (player) player->GetPosition(currentPlayerX, currentPlayerY);

		if (tileMap)
		{
			float left, top, right, bottom, leftEdge, rightEdge;

			player->GetBoundingBox(left, top, right, bottom);

			float playerBoundingBoxWidth = right - left;

			if (tileMap->GetWidth() < settingManager->GetIntValue("SCREEN_WIDTH"))
			{
				cx = 0;

				leftEdge = 0.0f;
				rightEdge = tileMap->GetWidth() - playerBoundingBoxWidth * 2;

				if (currentPlayerX >= rightEdge) {
					player->SetPosition(rightEdge, currentPlayerY);
				}

				if (currentPlayerX <= leftEdge) {
					player->SetPosition(leftEdge, currentPlayerY);
				}
			}
			else
			{
				if (currentPlayerX < settingManager->GetIntValue("SCREEN_WIDTH") / 2 - playerBoundingBoxWidth)
				{
					cx = 0.0f;
					leftEdge = 0.0f;

					if (currentPlayerX <= leftEdge) {
						player->SetPosition(leftEdge, currentPlayerY);
					}
				}
				else if (currentPlayerX >= tileMap->GetWidth() - settingManager->GetIntValue("SCREEN_WIDTH") / 2 - playerBoundingBoxWidth)
				{
					cx = tileMap->GetWidth() - settingManager->GetIntValue("SCREEN_WIDTH");

					rightEdge = tileMap->GetWidth() - playerBoundingBoxWidth * 2;

					if (currentPlayerX >= rightEdge) {
						player->SetPosition(rightEdge, currentPlayerY);
					}
				}
				else
				{
					cx = currentPlayerX + playerBoundingBoxWidth - settingManager->GetIntValue("SCREEN_WIDTH") / 2;
					rightEdge = CGame::GetInstance()->GetCamera()->GetRight() - playerBoundingBoxWidth * 2;
					leftEdge = CGame::GetInstance()->GetCamera()->GetLeft();

					if (currentPlayerX >= rightEdge) {
						player->SetPosition(rightEdge, currentPlayerY);
					}

					if (currentPlayerX <= leftEdge) {
						player->SetPosition(leftEdge, currentPlayerY);
					}
				}
			}

			cy = 0.0f;
		}

		// Update camera position
		game->GetCamera()->SetPosition(cx, cy);

		// PauseBadge
		if (pauseBadge)
		{
			pauseBadge->SetPosition(game->GetCamera()->GetLeft() + (settingManager->GetIntValue("SCREEN_WIDTH") / 2) - 120, game->GetCamera()->GetTop() + (SCREEN_HEIGHT / 2) - 60);
		}
	}

	// Update blackboard position
	if (blackboard)
	{
		blackboard->Update();
	}

	// Update game over board
	if (gameOverBoard)
	{
		gameOverBoard->Update();
	}
}

void CPlayScene::Render()
{
	// TileMap
	if (tileMap && !showingGameOverBoard)
	{
		tileMap->Render(game->GetCamera());
	}

	// Grounds
	if (!showingGameOverBoard)
	{
		for (int i = 0; i < grounds.size(); i++)
		{
			grounds[i]->Render();
		}
	}

	// Game objects
	sort(objects.begin(), objects.end(), CGameObject::CompareElevation);

	if (!showingGameOverBoard)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		}
	}

	// Blackboard
	if (blackboard)
	{
		blackboard->Render();
	}

	// PauseBadge
	if (hardPaused && showingPauseBadge && pauseBadge)
	{
		pauseBadge->Render();
	}

	// RetryBoard
	if (showingGameOverBoard && gameOverBoard)
	{
		gameOverBoard->Render();
	}
}

void CPlayScene::Unload()
{
	SAFE_DELETE(blackboard);
	SAFE_DELETE(tileMap);

	for (int i = 0; i < grounds.size(); i++)
	{
		SAFE_DELETE(grounds[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		SAFE_DELETE(objects[i]);
	}

	SAFE_DELETE(pauseBadge);

	SAFE_DELETE(gameOverBoard);

	grounds.clear();

	objects.clear();

	coObjects.clear();

	createdObjects.clear();

	player = nullptr;

	SAFE_DELETE(grid);
}

CSimon* CPlayScene::GetPlayer()
{
	return this->player;
}

void CPlaySceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CScene* scene = game->GetSceneManager()->GetCurrentScene();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();
	CPlayerData* playerData = game->GetPlayerData();

	if (simon != nullptr && !scene->HardPaused() && !((CPlayScene*)scene)->ShowingGameOverBoard())
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
			simon->down = true;

			if (!simon->onStair)
			{
				simon->SetState(SIMON_STATE_SIT);
			}
		}

		if (game->GetInputManager()->IsKeyDown(DIK_UP))
		{
			simon->up = true; 

			if (!simon->onStair)
			{
				simon->SetState(SIMON_STATE_IDLE);
			}
			else
			{
				if (simon->down && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
				{
					return;
				}

				if (simon->stairDirectionY == Direction::Up)
				{
					if (simon->stairDirectionX == Direction::Right)
					{
						OnHoldRightKey();
					}
					else
					{
						OnHoldLeftKey();
					}
				}
				else
				{
					if (simon->stairDirectionX == Direction::Right)
					{
						OnHoldLeftKey();
					}
					else
					{
						OnHoldRightKey();
					}
				}
			}
		}
		
		else if (game->GetInputManager()->IsKeyDown(DIK_RIGHT))
		{
			OnHoldRightKey();
		}
		else if (game->GetInputManager()->IsKeyDown(DIK_LEFT))
		{
			OnHoldLeftKey();
		}
		else if (game->GetInputManager()->IsKeyDown(DIK_DOWN))
		{
			if (!simon->onStair)
			{
				simon->SetState(SIMON_STATE_SIT);
			}
			else
			{
				if (simon->stairDirectionY == Direction::Up)
				{
					if (simon->stairDirectionX == Direction::Right)
					{
						OnHoldLeftKey();
					}
					else
					{
						OnHoldRightKey();
					}
				}
				else
				{
					if (simon->stairDirectionX == Direction::Right)
					{
						OnHoldRightKey();
					}
					else
					{
						OnHoldLeftKey();
					}
				}
			}
		}
		else
		{
			if (!game->Ended())
			{
				if (game->GetTimer()->GetRemainingTime() > 0 && playerData->GetHealthVolumes() > 0 && playerData->GetLives() >= 0)
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
			else
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
		}
	}
}

void CPlaySceneKeyHandler::OnKeyDown(int keyCode)
{
	CGame* game = CGame::GetInstance();
	CScene* scene = game->GetSceneManager()->GetCurrentScene();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();
	CPlayerData* playerData = game->GetPlayerData();
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (simon != nullptr && !scene->HardPaused() && !((CPlayScene*)scene)->ShowingGameOverBoard())
	{
		switch (keyCode)
		{
		case DIK_F1:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("INTRO_SCENE"));
			break;

		case DIK_F2:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("CUT_SCENE_1"));
			break;

		case DIK_F3:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("PLAY_SCENE_1"));
			break;

		case DIK_F4:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("CUT_SCENE_2"));
			break;

		case DIK_F5:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(game->GetSceneManager()->GetCurrentSceneIndex());
			game->GetPlayerData()->Reset();
			break;

		case DIK_F6:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("PLAY_SCENE_2_1"));
			break;

		case DIK_F7:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("PLAY_SCENE_2_2"));
			break;

		case DIK_F8:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("CUT_SCENE_3"));
			break;

		case DIK_F9:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("PLAY_SCENE_3_1"));
			break;

		case DIK_F10:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("PLAY_SCENE_3_2"));
			break;

		case DIK_F11:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("PLAY_SCENE_4"));
			break;

		case DIK_F12:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(settingManager->GetIntValue("CREDITS_SCENE"));
			break;

		case DIK_N:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(game->GetSceneManager()->GetNextSceneIndex());
			break;

		case DIK_P:
			if (!game->CheatKeysActivated()) return;
			game->GetSceneManager()->SwitchSceneByIndex(game->GetSceneManager()->GetPreviousSceneIndex());
			break;

		case DIK_1:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetWhipLevel() == WHIP_LEVEL_2)
			{
				game->GetPlayerData()->SetWhipLevel(WHIP_LEVEL_1);
			}
			else
			{
				game->GetPlayerData()->SetWhipLevel(WHIP_LEVEL_2);
			}

			break;

		case DIK_2:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetWhipLevel() == WHIP_LEVEL_3)
			{
				game->GetPlayerData()->SetWhipLevel(WHIP_LEVEL_1);
			}
			else
			{
				game->GetPlayerData()->SetWhipLevel(WHIP_LEVEL_3);
			}

			break;

		case DIK_3:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetPower() == DOUBLE_POWER)
			{
				game->GetPlayerData()->SetPower(NORMAL_POWER);
			}
			else
			{
				game->GetSoundManager()->Play("collecting_weapon_item");
				game->GetPlayerData()->SetPower(DOUBLE_POWER);
			}

			break;

		case DIK_4:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetPower() == TRIPLE_POWER)
			{
				game->GetPlayerData()->SetPower(NORMAL_POWER);
			}
			else
			{
				game->GetSoundManager()->Play("collecting_weapon_item");
				game->GetPlayerData()->SetPower(TRIPLE_POWER);
			}

			break;

		case DIK_5:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetSubWeaponType() == "dagger")
			{
				game->GetPlayerData()->SetSubWeaponType("");
			}
			else
			{
				game->GetSoundManager()->Play("collecting_weapon_item");
				game->GetPlayerData()->SetSubWeaponType("dagger");
			}

			break;

		case DIK_6:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetSubWeaponType() == "boomerang")
			{
				game->GetPlayerData()->SetSubWeaponType("");
			}
			else
			{
				game->GetSoundManager()->Play("collecting_weapon_item");
				game->GetPlayerData()->SetSubWeaponType("boomerang");
			}

			break;

		case DIK_7:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetSubWeaponType() == "axe")
			{
				game->GetPlayerData()->SetSubWeaponType("");
			}
			else
			{
				game->GetSoundManager()->Play("collecting_weapon_item");
				game->GetPlayerData()->SetSubWeaponType("axe");
			}

			break;

		case DIK_8:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetSubWeaponType() == "holy_water")
			{
				game->GetPlayerData()->SetSubWeaponType("");
			}
			else
			{
				game->GetSoundManager()->Play("collecting_weapon_item");
				game->GetPlayerData()->SetSubWeaponType("holy_water");
			}

			break;

		case DIK_9:
			if (!game->CheatKeysActivated()) return;

			if (game->GetPlayerData()->GetSubWeaponType() == "stopwatch")
			{
				game->GetPlayerData()->SetSubWeaponType("");
			}
			else
			{
				game->GetSoundManager()->Play("collecting_weapon_item");
				game->GetPlayerData()->SetSubWeaponType("stopwatch");
			}

			break;

		case DIK_0:
			if (!game->CheatKeysActivated()) return;
			game->GetPlayerData()->ResetWeapons();
			break;

		case DIK_BACKSPACE:
			if (!game->CheatKeysActivated()) return;
			game->GetPlayerData()->Reset();
			break;

		case DIK_H:
			if (!game->CheatKeysActivated()) return;
			game->GetPlayerData()->ResetHealthVolumes();
			break;

		case DIK_I:
			if (!game->CheatKeysActivated()) return;

			if (!simon->fullyInvisible)
			{
				game->GetSoundManager()->Play("starting_invisibility");
			}
			else
			{
				game->GetSoundManager()->Play("ending_invisibility");
			}

			simon->fullyInvisible = !simon->fullyInvisible;
			
			break;

		case DIK_L:
			if (!game->CheatKeysActivated()) return;
			game->GetPlayerData()->AddLives(settingManager->GetIntValue("ADDED_LIVES"));
			break;

		case DIK_R:
			if (!game->CheatKeysActivated()) return;
			game->GetPlayerData()->AddHearts(settingManager->GetIntValue("ADDED_HEARTS"));
			break;

		case DIK_W:
			game->SetPauseStartingTime(GetTickCount());
			game->GetSceneManager()->GetCurrentScene()->HardPause();
			game->GetSoundManager()->StopBackgroundSounds();
			game->GetSoundManager()->Play("pausing");
			break;

		case DIK_Z:
			if (simon->GetState() == SIMON_STATE_STAND_ON_STAIR_AND_THROW && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
			{
				return;
			}

			if (simon->GetState() == SIMON_STATE_STAND_AND_THROW && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
			{
				return;
			}

			if ((simon->GetState() == SIMON_STATE_WALK_UPSTAIR_AND_ATTACK || simon->GetState() == SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK) && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
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

				if (playerData->GetSubWeaponType() != "" && playerData->GetHearts() > 0 && simon->AbleToThrowSubWeapon() && simon->up)
				{
					simon->SetState(SIMON_STATE_STAND_ON_STAIR_AND_THROW);
				}
			}
			else
			{
				if (playerData->GetSubWeaponType() != "" && playerData->GetHearts() > 0 && simon->AbleToThrowSubWeapon() && simon->up)
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
			if (simon->onStair || simon->sitting)
			{
				return;
			}

			if (simon->GetState() == SIMON_STATE_STAND_AND_ATTACK && !simon->touchingGround && !simon->animationSet->at(simon->GetAnimationToRender())->Over())
			{
				return;
			}

			if (simon->jumpable)
			{
				simon->SetState(SIMON_STATE_JUMP);
			}

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
			simon->down = true;

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
	else
	{
		if (!game->Ended())
		{
			switch (keyCode)
			{
			case DIK_W:
				game->SetPauseEndingTime(GetTickCount());
				game->GetSceneManager()->GetCurrentScene()->ResumeHardPause();
				game->GetSoundManager()->PlayBackgroundSounds();
				break;

			default:
				break;
			}
		}

		if (((CPlayScene*)scene)->ShowingGameOverBoard())
		{
			switch (keyCode)
			{
			case DIK_UP:
				((CPlayScene*)scene)->GetGameOverBoard()->Up();
				break;

			case DIK_DOWN:
				((CPlayScene*)scene)->GetGameOverBoard()->Down();
				break;

			case DIK_RETURN:
			case DIK_W:
				((CPlayScene*)scene)->GetGameOverBoard()->Select();
				break;
			}
		}
	}
}

void CPlaySceneKeyHandler::OnKeyUp(int keyCode)
{
	CGame* game = CGame::GetInstance();
	CScene* scene = game->GetSceneManager()->GetCurrentScene();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();

	if (simon != nullptr && !scene->HardPaused() && !((CPlayScene*)scene)->ShowingGameOverBoard())
	{
		switch (keyCode)
		{
		case DIK_UP:
			simon->up = false;
			break;

		case DIK_DOWN:
			simon->down = false;
			break;
		}
	}
}

void CPlaySceneKeyHandler::OnHoldLeftKey()
{
	CGame* game = CGame::GetInstance();
	CScene* scene = game->GetSceneManager()->GetCurrentScene();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();

	simon->SetDirectionX(Direction::Left);

	if (!simon->sitting && !simon->onStair)
	{
		simon->SetState(SIMON_STATE_WALK);
	}

	if (simon->onStair)
	{
		if (simon->directionX == simon->stairDirectionX)
		{
			if (simon->stairDirectionY == Direction::Up)
			{
				simon->SetState(SIMON_STATE_WALK_UPSTAIR);
			}
			else
			{
				simon->SetState(SIMON_STATE_WALK_DOWNSTAIR);
			}
		}
		else
		{
			if (simon->stairDirectionY == Direction::Up)
			{
				simon->SetState(SIMON_STATE_WALK_DOWNSTAIR);
			}
			else
			{
				simon->SetState(SIMON_STATE_WALK_UPSTAIR);
			}
		}
	}
}

void CPlaySceneKeyHandler::OnHoldRightKey()
{
	CGame* game = CGame::GetInstance();
	CScene* scene = game->GetSceneManager()->GetCurrentScene();
	CSimon* simon = ((CPlayScene*)scene)->GetPlayer();

	simon->SetDirectionX(Direction::Right);

	if (!simon->sitting && !simon->onStair)
	{
		simon->SetState(SIMON_STATE_WALK);
	}

	if (simon->onStair)
	{
		if (simon->directionX == simon->stairDirectionX)
		{
			if (simon->stairDirectionY == Direction::Up)
			{
				simon->SetState(SIMON_STATE_WALK_UPSTAIR);
			}
			else
			{
				simon->SetState(SIMON_STATE_WALK_DOWNSTAIR);
			}
		}
		else
		{
			if (simon->stairDirectionY == Direction::Up)
			{
				simon->SetState(SIMON_STATE_WALK_DOWNSTAIR);
			}
			else
			{
				simon->SetState(SIMON_STATE_WALK_UPSTAIR);
			}
		}
	}
}
