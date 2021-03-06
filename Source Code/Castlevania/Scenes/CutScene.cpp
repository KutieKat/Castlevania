#include "CutScene.h"
#include "../Models/ObjectFactory.h"
#include "../Utilities/SafeDelete.h"

void CCutScene::ParseSounds(TiXmlElement * element)
{
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
			CGameSoundManager::GetInstance()->Add(id, path, loop, retained, isBackground);
		}
	}
}

void CCutScene::ParseTextures(TiXmlElement* element)
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

void CCutScene::ParseSprites(TiXmlElement* element)
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

void CCutScene::ParseAnimations(TiXmlElement* element)
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

void CCutScene::ParseAnimationSets(TiXmlElement* element)
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

void CCutScene::ParseObjects(TiXmlElement* element)
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

		CAnimationSet* animationSet = CAnimationSets::GetInstance()->Get(animationSetId);
		CGameObject* gameObject = nullptr;

		if (type == "simon")
		{
			bool standingToWatch;
			object->QueryBoolAttribute("standingToWatch", &standingToWatch);

			int waitingTime;
			object->QueryIntAttribute("waitingTime", &waitingTime);

			player = new CSimon();
			player->movingDirection = object->Attribute("movingDirection");
			player->switchScenePosition = object->Attribute("switchScenePosition");
			player->standingToWatch = standingToWatch;
			player->waitingTime = waitingTime;
			player->SetState(SIMON_STATE_CUT_SCENE_AUTO_WALK);
			player->SetPosition(x, y);

			objects.emplace_back(player);
		}
		else
		{
			gameObject = CObjectFactory::Construct(type);
		}

		if (gameObject)
		{
			gameObject->SetId(id);
			gameObject->SetPosition(x, y);

			if (type == "bat")
			{
				string trajectory = "";

				if (object->Attribute("trajectory"))
				{
					trajectory = object->Attribute("trajectory");
				}
				
				if (trajectory == "right_cross")
				{
					gameObject->SetState(BAT_STATE_FLY_RIGHT_CROSS);
				}
				else if (trajectory == "oval")
				{
					gameObject->SetState(BAT_STATE_FLY_OVAL);
				}
			}

			objects.emplace_back(gameObject);
		}
	}
}

CCutScene::CCutScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId, string requiredSceneId) : CScene(id, filePath, stage, previousSceneId, nextSceneId, requiredSceneId)
{
	game = CGame::GetInstance();
	keyHandler = new CCutSceneKeyHandler(this);
}

bool CCutScene::Load()
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Error(doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* sounds = root->FirstChildElement();
	TiXmlElement* textures = sounds->NextSiblingElement();
	TiXmlElement* sprites = textures->NextSiblingElement();
	TiXmlElement* animations = sprites->NextSiblingElement();
	TiXmlElement* animationSets = animations->NextSiblingElement();
	TiXmlElement* objects = animationSets->NextSiblingElement();

	ParseSounds(sounds);
	ParseTextures(textures);
	ParseSprites(sprites);
	ParseAnimations(animations);
	ParseAnimationSets(animationSets);
	ParseObjects(objects);

	blackboard = new CBlackboard();

	CGame::GetInstance()->GetSoundManager()->PlayBackgroundSounds();

	return true;
}

bool CCutScene::Reload()
{
	return Load();
}

void CCutScene::Update(DWORD dt)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetVisibility() == Visibility::Visible)
		{
			objects[i]->Update(dt, nullptr);
		}
	}

	game->GetCamera()->SetPosition(0, 0.0f);

	if (blackboard)
	{
		blackboard->Update();
	}
}

void CCutScene::Render()
{
	sort(objects.begin(), objects.end(), CGameObject::CompareElevation);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	if (blackboard)
	{
		blackboard->Render();
	}
}

void CCutScene::Unload()
{
	SAFE_DELETE(blackboard);

	for (int i = 0; i < objects.size(); i++)
	{
		SAFE_DELETE(objects[i]);
	}

	objects.clear();

	player = nullptr;
}

void CCutSceneKeyHandler::KeyState(BYTE* states)
{
}

void CCutSceneKeyHandler::OnKeyDown(int keyCode)
{
	CGame* game = CGame::GetInstance();
	CSceneManager* sceneManager = game->GetSceneManager();
	CSettingManager* settingManager = CSettingManager::GetInstance();

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
	}
}

void CCutSceneKeyHandler::OnKeyUp(int keyCode)
{
}