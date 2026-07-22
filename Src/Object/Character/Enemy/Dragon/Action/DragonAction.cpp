#include "../../../../../Manager/SceneManager.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Resource/JsonResource.h"
#include "Update/DragonActionUpdateBase.h"
#include "Update/DragonActionUpdateFlyMove.h"
#include "DragonAction.h"

DragonAction::DragonAction(nlohmann::json jsonData, std::shared_ptr<Transform> transform,float blockSize)
{
	sumWeight_ = 0;
	transform_ = transform;
	time_  = 0.0f;
	jsonData_ = jsonData;
	blockSize_ = blockSize;
	LoadJsonData();
	Init();
}

DragonAction::~DragonAction(void)
{
}

void DragonAction::Init(void)
{
	time_ = 0.0f;
	SetNextActionId();
}

void DragonAction::Update(void)
{
	update_->Update();
	time_ += SceneManager::GetInstance().GetDeltaTime();
}

void DragonAction::Draw(void)
{
}

void DragonAction::UIDraw(void)
{
}

void DragonAction::SetNextActionId(void)
{
	//重みづけされた中から次の行動を決める
	int randomValue = rand() % sumWeight_;
	int accumulatedWeight = 0;
	for (const auto& nextAction : nextActions_)
	{
		accumulatedWeight += nextAction.weight;
		if (randomValue < accumulatedWeight)
		{
			nextActionId_ = nextAction.id;
			break;
		}
	}
}

void DragonAction::LoadJsonData(void)
{
	// 自分の情報を取得する
	id_ = jsonData_["Id"];
	action_ = static_cast<Action>(id_);
	name_ = jsonData_["Name"];
	duration_ = jsonData_["Duration"];
	SetUpdateFunc(action_);
	for (auto& nextActionJson : jsonData_["Next"])
	{
		// 次のアクションの情報を取得する
		NextActionInfo nextActionInfo;
		nextActionInfo.id = nextActionJson["Id"];
		nextActionInfo.weight = nextActionJson["Weight"];
		nextActionInfo.nextAction = static_cast<Action>(nextActionInfo.id);
		sumWeight_ += nextActionInfo.weight;
		nextActions_.push_back(nextActionInfo);
	}
}

void DragonAction::SetUpdateFunc(Action action)
{
	auto& resourceManager = ResourceManager::GetInstance();
	switch (action)
	{
	case DragonAction::Action::NONE:
		break;
	case DragonAction::Action::FLY_MOVE:
		update_ = std::make_unique<DragonActionUpdateFlyMove>(transform_, resourceManager.GetJsonResource(jsonData_["Json"]).lock()->GetData(), *this);
		break;
	case DragonAction::Action::FLY_BREATH_ATTACK:
		//updateFunc_ = std::bind(&DragonAction::UpdateFlyBreathAttack, this);
		break;
	case DragonAction::Action::LANDING:
		//updateFunc_ = std::bind(&DragonAction::UpdateLanding, this);
		break;
	case DragonAction::Action::LAND_ATTACK:
		//updateFunc_ = std::bind(&DragonAction::UpdateLandAttack, this);
		break;
	case DragonAction::Action::TAKE_FLIGHT:
		//updateFunc_ = std::bind(&DragonAction::UpdateTakeFlight, this);
		break;
	case DragonAction::Action::FLY_PLAYER_ATTACK:
		//updateFunc_ = std::bind(&DragonAction::UpdateFlyPlayerAttack, this);
		break;
	default:
		break;
	}
}
