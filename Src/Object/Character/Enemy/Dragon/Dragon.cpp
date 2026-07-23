#include "../../../../Utility/Utility.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/Resource/JsonResource.h"
#include "../../../Common/Geometry/Triangle3D.h"
#include "../../../Common/Geometry/Capsule.h"
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
	InitCollider();
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
	UpdateFramePos();
}

void Dragon::Draw(void)
{
	actionManager_->Draw();
	model_->Draw();
	//for (auto& param : colParam_)
	//{
	//	param.geometry_->Draw();
	//}
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

void Dragon::InitCollider(void)
{
	Collider::TAG tag = Collider::TAG::ENEMY;
	std::vector<Collider::TAG> notHitTags;
	notHitTags.push_back(Collider::TAG::ENEMY);
	notHitTags.push_back(Collider::TAG::ENEMY_ATTACK);
	std::unique_ptr<Geometry> geo;

	for (auto& collider : paramsJson_["Collider"])
	{
		std::string type = collider["Type"];
		if (type == "Triangle")
		{
			auto& bornNums = collider["BornNumber"];
			geo = std::make_unique<Triangle3D>(bornPos_[bornNums[0]], bornPos_[bornNums[1]], bornPos_[bornNums[2]]);
			MakeCollider(tag, std::move(geo), notHitTags);
			continue;
		}
		else if (type == "Capsule")
		{
			auto& bornNums = collider["BornNumber"];
			float radius = collider["Radius"];
			geo = std::make_unique<Capsule>(bornPos_[bornNums[0]], bornPos_[bornNums[1]], radius);
			MakeCollider(tag, std::move(geo), notHitTags);
			continue;
		}
	}
}

void Dragon::UpdateFramePos(void)
{
	int modelId = transform_->modelId;
	for (auto& bornPos : bornPos_)
	{
		bornPos.second = MV1GetFramePosition(modelId, bornPos.first);
	}
}
