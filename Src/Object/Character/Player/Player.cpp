#include <fstream>
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource/JsonResource.h"
#include "../../../Application.h"
#include "PlayerModel.h"
#include "Player.h"

Player::Player(std::string skinName)
{
	LoadPlayerInfo();
	if (skinName == "")
	{
		skinName = "Alex";
	}
	transform_ = std::make_shared<Transform>();
	model_ = std::make_unique<PlayerModel>(skinName, paramsJson_);
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	model_->Init();
}

void Player::Update(void)
{
	model_->Update();
}

void Player::Draw(void)
{
	model_->Draw();
}

void Player::UIDraw(void)
{
	model_->UIDraw();
}

void Player::SetSkinHandle(int handle)
{
	model_->SetSkinHandle(handle);
}

void Player::SetAnimation(std::string animName, bool isCompulsion)
{
	model_->SetAnimation(animName, isCompulsion);
}

void Player::ApplyVertex(void)
{
	model_->ApplayVertexPos(transform_->pos, transform_->rot);
}

void Player::LoadPlayerInfo(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource("PlayerJson").lock();
	paramsJson_ = jsonResource->GetData();
	params_.COLLISION_SIZE = FLOAT3(paramsJson_["Collision"]["Width"], paramsJson_["Collision"]["Height"], paramsJson_["Collision"]["Depth"]);
	params_.MAX_HEALTH = paramsJson_["Status"]["MaxHealth"];
	params_.BLOCK_REACH = paramsJson_["Reach"]["Block"];
	params_.ENTITY_REACH = paramsJson_["Reach"]["Entity"];
	params_.WALK_SPEED = paramsJson_["Move"]["WalkSpeed"];
	params_.SPRINT_SPEED = paramsJson_["Move"]["SprintSpeed"];
	params_.JUMP_POWER = paramsJson_["Move"]["JumpVelocity"];
	params_.health = paramsJson_["Status"]["Health"];
}
