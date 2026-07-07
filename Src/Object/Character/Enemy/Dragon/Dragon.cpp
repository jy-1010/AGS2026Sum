#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource/JsonResource.h"
#include "DragonModel.h"
#include "Dragon.h"

Dragon::Dragon(void)
{
	LoadDragonInfo();
	transform_ = std::make_shared<Transform>();
	model_ = std::make_unique<DragonModel>(transform_, paramsJson_);
}

Dragon::~Dragon(void)
{
}

void Dragon::Init(void)
{
}

void Dragon::Update(void)
{
	model_->Update();
}

void Dragon::Draw(void)
{
	model_->Draw();
}

void Dragon::UIDraw(void)
{
	model_->UIDraw();
}

void Dragon::LoadDragonInfo(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource("EnemyDragonJson").lock();
	paramsJson_ = jsonResource->GetData();
	params_.MAX_HEALTH = paramsJson_["Status"]["MaxHealth"];
	params_.health = paramsJson_["Status"]["Health"];
	params_.DEFAULT_SCALE = paramsJson_["DefaultScale"];
}
