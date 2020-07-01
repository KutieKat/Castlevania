#include "CreditsScene.h"
#include "../Models/ObjectFactory.h"
#include "../Utilities/SafeDelete.h"

void CCreditsScene::ParseSounds(TiXmlElement* element)
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

void CCreditsScene::ParseTextures(TiXmlElement* element)
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

void CCreditsScene::ParseSprites(TiXmlElement* element)
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

void CCreditsScene::ParseAnimations(TiXmlElement* element)
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

void CCreditsScene::ParseAnimationSets(TiXmlElement* element)
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

void CCreditsScene::ParseObjects(TiXmlElement* element)
{
	TiXmlElement* object = nullptr;

	for (object = element->FirstChildElement(); object != nullptr; object = object->NextSiblingElement())
	{
		bool visible = true;
		object->QueryBoolAttribute("visible", &visible);

		int x, y;
		object->QueryIntAttribute("x", &x);
		object->QueryIntAttribute("y", &y);

		string id = object->Attribute("id");
		string type = object->Attribute("type");
		string animationSetId = object->Attribute("animationSetId");

		CAnimationSet* animationSet = CAnimationSets::GetInstance()->Get(animationSetId);
		CGameObject* gameObject = CObjectFactory::Construct(type);

		if (gameObject)
		{
			gameObject->SetId(id);
			gameObject->SetPosition(x, y);
			gameObject->SetVisibility(visible == true ? Visibility::Visible : Visibility::Hidden);

			objects.emplace_back(gameObject);
		}
	}
}

CCreditsScene::CCreditsScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId, string requiredSceneId) : CScene(id, filePath, stage, previousSceneId, nextSceneId, requiredSceneId)
{
	game = CGame::GetInstance();
	keyHandler = new CCreditsSceneKeyHandler(this);
}

bool CCreditsScene::Load()
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

	CGame::GetInstance()->GetSoundManager()->Play("knocked_down");

	return true;
}

bool CCreditsScene::Reload()
{
	return Load();
}

void CCreditsScene::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	CSceneManager* sceneManager = game->GetSceneManager();

	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CCastle*>(objects[i]))
		{
			if (objects[i]->animationSet->at(0)->Over())
			{
				game->GetSoundManager()->Stop("knocked_down");
				game->GetSoundManager()->PlayBackgroundSounds();

				for (int j = 0; j < objects.size(); j++)
				{
					if (dynamic_cast<CCreditText*>(objects[j]))
					{
						if (objects[j]->GetVisibility() == Visibility::Hidden)
						{
							objects[j]->animationSet->at(0)->SetStartTime(GetTickCount());
							objects[j]->SetVisibility(Visibility::Visible);
						}
					}
				}
			}
		}
		else if (dynamic_cast<CCreditText*>(objects[i]))
		{
			if (objects[i]->animationSet->at(0)->Over())
			{
				sceneManager->SwitchScene(sceneManager->GetFirstSceneId());
				break;
			}
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetVisibility() == Visibility::Visible)
		{
			objects[i]->Update(dt, nullptr);
		}
	}

	game->GetCamera()->SetPosition(0, 0.0f);
}

void CCreditsScene::Render()
{
	sort(objects.begin(), objects.end(), CGameObject::CompareElevation);

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetVisibility() == Visibility::Visible)
		{
			objects[i]->Render();
		}
	}
}

void CCreditsScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		SAFE_DELETE(objects[i]);
	}

	objects.clear();
}

void CCreditsScene::SetSwitchSceneTime(DWORD time)
{
	switchSceneTime = time;
}

void CCreditsSceneKeyHandler::KeyState(BYTE* states)
{
}

void CCreditsSceneKeyHandler::OnKeyDown(int keyCode)
{
	CSceneManager* sceneManager = CGame::GetInstance()->GetSceneManager();

	if (keyCode)
	{
		sceneManager->SwitchScene(sceneManager->GetFirstSceneId());
	}
}

void CCreditsSceneKeyHandler::OnKeyUp(int keyCode)
{
}