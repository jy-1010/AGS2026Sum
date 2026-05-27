#include "Sound2DResource.h"

Sound2DResource::Sound2DResource(nlohmann::json json) : SoundResource(json)
{
    LoadResourceInfo();
    if (IsPreLoad())
    {
        SetUseASyncLoadFlag(true);
        Load();
        SetUseASyncLoadFlag(false);
	}
}

Sound2DResource::~Sound2DResource(void)
{
}

bool Sound2DResource::Load(void)
{
    return false;
}

void Sound2DResource::LoadResourceInfo(void)
{
    Init();
}

void Sound2DResource::Init(void)
{
	resourceType_ = TYPE::SOUND_2D;
}
