#include <fstream>
#include "JsonResource.h"

JsonResource::JsonResource(nlohmann::json json) : Resource(json)
{
	LoadResourceInfo();
	if(IsPreLoad())
	{
		Load();
	}
}

JsonResource::~JsonResource(void)
{
}

bool JsonResource::Load(void)
{
	std::fstream jsonFile(path_);
	if (!jsonFile.is_open())
	{
		isLoaded_ = false;
		return false;
	}

	jsonFile >> data_;
	isLoaded_ = true;
	return true;
}

bool JsonResource::IsLoaded(void) const
{
	return isLoaded_;
}

void JsonResource::LoadResourceInfo(void)
{
	Init();
}

void JsonResource::Init(void)
{
	isLoaded_ = false;
	resourceType_ = TYPE::JSON;
}
