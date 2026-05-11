#pragma once
#include "../../ObjectBase.h"

class PlayerModel;

class Player :  public ObjectBase
{
public:

	struct Param
	{
		FLOAT3 COLLISION_SIZE;
		float MAX_HEALTH;
		float BLOCK_REACH;
		float ENTITY_REACH;
		float WALK_SPEED;
		float SPRINT_SPEED;
		float JUMP_POWER;
		float health;
	};

	Player(std::string skinName);
	~Player(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UIDraw(void) override;

private:

	std::unique_ptr<PlayerModel> model_;
	nlohmann::json paramsJson_;
	Param params_;

	void LoadPlayerInfo(void);
};

