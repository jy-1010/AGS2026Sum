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
	selectedSkinName_ = "";
	auto jsonResource = resourceManager.GetJsonResource("PlayerSkinJson").lock();
	skinJson_ = jsonResource->GetData();
	rootPath_ = Application::PATH_DATA + skinJson_["RootPath"].get<std::string>();
	bool isPreload = skinJson_["Preload"];
	defaultSkinName_ = skinJson_["Default"];
	//lastSavename_ = "";
	//それぞれのスキンのインスタンス作成
	for (auto& skinInfo : skinJson_["Info"])
	{
		std::string name = skinInfo["Name"].get<std::string>();
		std::string path = rootPath_ + skinInfo["Path"].get<std::string>();
		AddSkin(name, path, isPreload);
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

std::vector<std::string> SkinManager::GetSkinNameAll(void)
{
	std::vector<std::string> ret;
	for (auto& skin : skinResources_)
	{
		ret.push_back(skin.first);
	}
	return ret;
}

const int SkinManager::GetSkinNum(void)
{
	return static_cast<int>(skinResources_.size());
}

void SkinManager::SaveSkin(const std::string name)
{
	AddSkin(name, rootPath_ + name + ".png", true);
	AddSkinJson(name, name + ".png");
	selectedSkinName_ = name;
}

std::string SkinManager::GetSelectedSkinName(void)
{
	if (selectedSkinName_ == "")
	{
		return defaultSkinName_;
	}
	return selectedSkinName_;
}

void SkinManager::AddSkin(const std::string name, const std::string path, bool isPreload)
{
	skinResources_[name] = std::make_shared<Skin>(name, path, isPreload);
}

void SkinManager::AddSkinJson(const std::string name, const std::string path)
{
	//中身を書き換える
	nlohmann::json json;
	json["Name"] = name;
	json["Path"] = path;
	skinJson_["Info"].push_back(json);
	skinJson_.dump(2);
	//情報を更新する
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource("PlayerSkinJson").lock();
	jsonResource->OutFile(skinJson_);
}
