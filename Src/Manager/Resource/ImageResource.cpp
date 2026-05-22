#include "ImageResource.h"

ImageResource::ImageResource(void)
{
	resourceType_ = TYPE::IMAGE;
	handleId_ = -1;
}

ImageResource::ImageResource(nlohmann::json json) : Resource(json)
{
	ImageResource();
	LoadResourceInfo();
}

ImageResource::~ImageResource(void)
{
}

bool ImageResource::Load(void)
{
	return false;
}

void ImageResource::LoadResourceInfo(void)
{

}
