#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/SceneManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(void)
{
}

SceneBase::~SceneBase(void)
{
}

bool SceneBase::Init(void)
{
	return true;
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}

void SceneBase::Load(void)
{
}

void SceneBase::UpdateIsPose(void)
{
	auto& keycon = KeyConfig::GetInstance();
	if (keycon.IsTrgDown(KeyConfig::CONTROL_TYPE::OPEN_MENU))
	{
		SceneManager::GetInstance().PushScene(SceneManager::SCENE_ID::MENU);
	}
}
