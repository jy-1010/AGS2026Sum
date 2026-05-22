#include <fstream>
#include "../Application.h"
#include "Resource/JsonResource.h"
#include "Resource/ImageResource.h"
#include "Resource/ImageArrayResource.h"
#include "Resource/ModelResource.h"
#include "Resource/Sound2DResource.h"
#include "Resource/Sound3DResource.h"
#include "Resource/ShaderResource.h"
#include "ResourceManager.h"


ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	std::fstream jsonFile(Application::PATH_JSON + "Resource.json");
	nlohmann::json json;
	jsonFile >> json;
	for (auto& info : json["Infos"].items())
	{
		std::string key = info.key();
		std::string typeStr = info.value()["Type"].get<std::string>();
		GetResourceTypeFromString(typeStr);
		std::shared_ptr<Resource> resource = CreateResource(info.value(), GetResourceTypeFromString(typeStr));
		resourcesMap_.emplace(key, resource);
	}
}

void ResourceManager::Release(void)
{
	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
	instance_ = nullptr;
}

Resource::TYPE ResourceManager::GetResourceTypeFromString(const std::string& str)
{
	if (str == "Json")
	{
		return Resource::TYPE::JSON;
	}
	else if(str == "Image")
	{
		return Resource::TYPE::IMAGE;
	}
	else if(str == "ImageArray")
	{
		return Resource::TYPE::IMAGEARRAY;
	}
	else if(str == "Model")
	{
		return Resource::TYPE::MODEL;
	}
	else if(str == "Sound2D")
	{
		return Resource::TYPE::SOUND_2D;
	}
	else if(str == "Sound3D")
	{
		return Resource::TYPE::SOUND_3D;
	}
	else if(str == "Shader")
	{
		return Resource::TYPE::SHADER;
	}
	return Resource::TYPE::NONE;
}

std::shared_ptr<Resource> ResourceManager::CreateResource( const nlohmann::json& json, Resource::TYPE type)
{
	switch (type)
	{
	case Resource::TYPE::JSON:
		return std::make_shared<JsonResource>(json);
	case Resource::TYPE::IMAGE:
		return std::make_shared<ImageResource>(json);
	case Resource::TYPE::IMAGEARRAY:
		return std::make_shared<ImageArrayResource>(json);
	case Resource::TYPE::MODEL:
		return std::make_shared<ModelResource>(json);
	case Resource::TYPE::SOUND_2D:
		return std::make_shared<Sound2DResource>(json);
	case Resource::TYPE::SOUND_3D:
		return std::make_shared<Sound3DResource>(json);
	case Resource::TYPE::SHADER:
		return std::make_shared<ShaderResource>(json);
	}
	return nullptr;
}