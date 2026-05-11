#include <fstream>
#include "../../../Application.h"
#include "PlayerModel.h"
#include "Player.h"

Player::Player(std::string skinName)
{
	LoadPlayerInfo();
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

void Player::LoadPlayerInfo(void)
{
	std::ifstream ifs(Application::PATH_JSON + "Player.json");
	if (!ifs)
	{
		std::cerr << "Failed to open PlayerInfo.json" << std::endl;
		return;
	}
	ifs >> paramsJson_;
	params_.COLLISION_SIZE = FLOAT3(paramsJson_["Collision"]["Width"], paramsJson_["Collision"]["Height"], paramsJson_["Collision"]["Depth"]);
	params_.MAX_HEALTH = paramsJson_["Status"]["MaxHealth"];
	params_.BLOCK_REACH = paramsJson_["Reach"]["Block"];
	params_.ENTITY_REACH = paramsJson_["Reach"]["Entity"];
	params_.WALK_SPEED = paramsJson_["Move"]["WalkSpeed"];
	params_.SPRINT_SPEED = paramsJson_["Move"]["SprintSpeed"];
	params_.JUMP_POWER = paramsJson_["Move"]["JumpVelocity"];
	params_.health = paramsJson_["Status"]["Health"];
}
