#include "../../../../Utility/Utility.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource/JsonResource.h"
#include "../../Player/Player.h"
#include "DragonModel.h"
#include "Action/DragonActionManager.h"
#include "Dragon.h"

Dragon::Dragon(const Player& player, float blockSize):player_(player)
{
	blockSize_ = blockSize;
	LoadDragonInfo();
	transform_ = std::make_shared<Transform>();
	transform_->quaRotLocal = Quaternion::Euler(VGet(0.0f, Utility::Deg2RadF(180.0f), 0.0f));
	model_ = std::make_unique<DragonModel>(transform_, paramsJson_);
	actionManager_ = std::make_unique<DragonActionManager>(paramsJson_["ActionPattern"],transform_,player_,blockSize_);
}

Dragon::~Dragon(void)
{
}

void Dragon::Init(void)
{
}

void Dragon::Update(void)
{
	actionManager_->Update();
	model_->Update();
}

void Dragon::Draw(void)
{
	actionManager_->Draw();
	model_->Draw();
}

void Dragon::UIDraw(void)
{
	actionManager_->UIDraw();
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
