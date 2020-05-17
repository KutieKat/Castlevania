#include "IntroScene.h"
#include "../Models/ObjectFactory.h"
#include "../Utilities/SafeDelete.h"

void CIntroScene::ParseTextures(TiXmlElement* element)
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

void CIntroScene::ParseSprites(TiXmlElement* element)
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

void CIntroScene::ParseAnimations(TiXmlElement* element)
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

void CIntroScene::ParseAnimationSets(TiXmlElement* element)
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

void CIntroScene::ParseObjects(TiXmlElement* element)
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
		CGameObject* gameObject = CObjectFactory::Construct(type);

		if (gameObject)
		{
			gameObject->SetId(id);
			gameObject->SetPosition(x, y);

			objects.emplace_back(gameObject);
		}
	}
}

CIntroScene::CIntroScene(string id, string filePath, string stage, string previousSceneId, string nextSceneId, string requiredSceneId) : CScene(id, filePath, stage, previousSceneId, nextSceneId, requiredSceneId)
{
	game = CGame::GetInstance();
	keyHandler = new CIntroSceneKeyHandler(this);
}

bool CIntroScene::Load()
{
	TiXmlDocument doc(filePath.c_str());

	if (!doc.LoadFile())
	{
		CDebug::Error(doc.ErrorDesc());
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* textures = root->FirstChildElement();
	TiXmlElement* sprites = textures->NextSiblingElement();
	TiXmlElement* animations = sprites->NextSiblingElement();
	TiXmlElement* animationSets = animations->NextSiblingElement();
	TiXmlElement* objects = animationSets->NextSiblingElement();

	ParseTextures(textures);
	ParseSprites(sprites);
	ParseAnimations(animations);
	ParseAnimationSets(animationSets);
	ParseObjects(objects);

	startLabel = new CLabel("PUSH START KEY", 146, 270, 17);

	return true;
}

bool CIntroScene::Reload()
{
	return Load();
}

void CIntroScene::Update(DWORD dt)
{
	if (switchSceneTime != -1)
	{
		interval += 1;

		if (GetTickCount() > switchSceneTime)
		{
			interval = 0;
			switchSceneTime = -1;

			game->GetSceneManager()->SwitchScene(GetNextSceneId());
		}

		if (startLabel)
		{
			if (interval >= 30 && interval <= 60 || interval >= 90 && interval <= 120 || interval >= 150 && interval <= 180)
			{
				startLabel->SetColor(CColor::Black);
			}
			else
			{
				startLabel->SetColor(CColor::White);
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

void CIntroScene::Render()
{
	if (startLabel) startLabel->Render();

	sort(objects.begin(), objects.end(), CGameObject::CompareElevation);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
}

void CIntroScene::Unload()
{
	SAFE_DELETE(startLabel);

	for (int i = 0; i < objects.size(); i++)
	{
		SAFE_DELETE(objects[i]);
	}

	objects.clear();
}

void CIntroScene::SetSwitchSceneTime(DWORD time)
{
	switchSceneTime = time;
}

void CIntroSceneKeyHandler::KeyState(BYTE* states)
{
}

void CIntroSceneKeyHandler::OnKeyDown(int keyCode)
{
	CGame* game = CGame::GetInstance();

	switch (keyCode)
	{
	case DIK_W:
		dynamic_cast<CIntroScene*>(game->GetSceneManager()->GetCurrentScene())->SetSwitchSceneTime(GetTickCount() + 3000);
		break;
	}
}

void CIntroSceneKeyHandler::OnKeyUp(int keyCode)
{
}