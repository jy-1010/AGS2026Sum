#include "Resource.h"

Resource::Resource(void)
{
}

Resource::Resource(nlohmann::json json)	: json_(json)
{
	LoadResourceInfo();
}

void Resource::LoadResourceInfo(void)
{
	path_ = json_["Path"].get<std::string>();

}
