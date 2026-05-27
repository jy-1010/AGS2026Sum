#include "../../../../Application.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource/JsonResource.h"
#include "Skin.h"
#include "SkinManager.h"

SkinManager* SkinManager::instance_ = nullptr;

void SkinManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SkinManager();
	}
	instance_->Init();
}

SkinManager& SkinManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CreateInstance();
	}
	return *instance_;
}

void SkinManager::Init(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource("PlayerSkinJson").lock();
	skinJson_ = jsonResource->GetData();
	std::string lootPath = Application::PATH_DATA + skinJson_["lootPath"].get<std::string>();
	bool isPreload = skinJson_["Preload"];
	for (auto& skinInfo : skinJson_["Info"])
	{
		std::string name = skinInfo["Name"].get<std::string>();
		std::string path = lootPath + skinInfo["Path"].get<std::string>();
		skinResources_[name] = std::make_shared<Skin>(name, path, isPreload);
	}
}

void SkinManager::Release(void)
{
	skinResources_.clear();
}

void SkinManager::Destroy(void)
{
	Release();
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

const int SkinManager::GetHandleId(const std::string key)
{
	if (!skinResources_[key]->isLoaded())
	{
		skinResources_[key]->Load();
	}
	return skinResources_[key]->GetHandle();
}

const int SkinManager::GetSkinNum(void)
{
	return static_cast<int>(skinResources_.size());
}
