#include <fstream>
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource/JsonResource.h"
#include "../../../Manager/KeyConfig.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Application.h"
#include "../../Common/Geometry/Capsule.h"
#include "Skin/SkinManager.h"
#include "PlayerModel.h"
#include "Player.h"

Player::Player(std::string skinName,float blockSize)
{
	LoadPlayerInfo();
	blockSize_ = blockSize;
	if (skinName == "")
	{
		skinName = SkinManager::GetInstance().GetDefaultSkinName();
	}
	transform_ = std::make_shared<Transform>();
	headTrans_ = std::make_shared<Transform>();
	model_ = std::make_unique<PlayerModel>(skinName, paramsJson_);
	InitCollider();
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
	UpdateMove();
	UpdateAttack();
	CalcHeadPos();
	model_->Update();
}

void Player::Draw(void)
{
	model_->Draw();
	for (auto& param : colParam_)
	{
		//param.geometry_->Draw();
	}
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
	model_->ApplayVertexPos(transform_->pos, transform_->rot,params_.DefaultScale);
}

const float Player::GetDamage(Collider::TAG tag)const
{
	float ret = 0.0f;
	switch (tag)
	{
	case Collider::TAG::PLAYER:
		break;
	case Collider::TAG::PLAYER_ATTACK_SWORD:
		ret = params_.SWORD_DAMAGE;
		break;
	case Collider::TAG::PLAYER_ATTACK_ARROW:
		ret = params_.ARROW_DAMAGE;
		break;
	case Collider::TAG::ENEMY:
		break;
	case Collider::TAG::ENEMY_ATTACK:
		break;
	default:
		break;
	}
	return ret;
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	std::shared_ptr<Collider> hitCol = _hitCol.lock();
	Collider::TAG tag = hitCol->GetTag();
	switch (tag)
	{
	case Collider::TAG::PLAYER_ATTACK_ARROW:
	case Collider::TAG::PLAYER_ATTACK_SWORD:
	case Collider::TAG::PLAYER:
		return;
		break;
	case Collider::TAG::ENEMY:
	case Collider::TAG::ENEMY_ATTACK:
		return;
		break;
	default:
		break;
	}
}

void Player::LoadPlayerInfo(void)
{
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource("PlayerJson").lock();
	paramsJson_ = jsonResource->GetData();
	params_.COLLISION_SIZE = FLOAT3(paramsJson_["Collision"]["Width"], paramsJson_["Collision"]["Height"], paramsJson_["Collision"]["Depth"]);
	params_.MAX_HEALTH = paramsJson_["Status"]["MaxHealth"];
	params_.HEAD_HEIGHT = paramsJson_["HeadHeight"];
	params_.BLOCK_REACH = paramsJson_["Reach"]["Block"];
	params_.ENTITY_REACH = paramsJson_["Reach"]["Entity"];
	params_.WALK_SPEED = paramsJson_["Move"]["WalkSpeed"];
	params_.SPRINT_SPEED = paramsJson_["Move"]["SprintSpeed"];
	params_.JUMP_POWER = paramsJson_["Move"]["JumpVelocity"];
	params_.SWORD_DAMAGE = paramsJson_["Damage"]["Sword"];
	params_.ARROW_DAMAGE = paramsJson_["Damage"]["Arrow"];
	params_.health = paramsJson_["Status"]["Health"];
	params_.DefaultScale = paramsJson_["DefaultScale"];
}

void Player::UpdateAttack(void)
{
	KeyConfig& keycon = KeyConfig::GetInstance();
	if (keycon.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK))
	{
		SetAnimation("Swing", false);
	}
}

void Player::UpdateMove(void)
{
	KeyConfig& keycon = KeyConfig::GetInstance();
	Camera& camera = SceneManager::GetInstance().GetCamera();

	//移動量
	float moveSpeed = 0.0f;
	bool isDash = false;
	if (keycon.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DASH))
	{
		isDash = true;
		moveSpeed = params_.SPRINT_SPEED * blockSize_ * SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		moveSpeed = params_.WALK_SPEED * blockSize_ * SceneManager::GetInstance().GetDeltaTime();
	}
	//カメラの前方向を取得
	VECTOR front = VSub(camera.GetTargetPos(), camera.GetPos());
	front.y = 0.0f;
	front = VNorm(front);
	//カメラの左方向を取得
	VECTOR left = front;
	std::swap(left.x, left.z);
	left.x = -left.x;
	VECTOR dir = Utility::VECTOR_ZERO;
	//キーボードでの移動処理
	if (keycon.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP))
	{
		dir = VAdd(dir, front);
	}
	if (keycon.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN))
	{
		dir = VAdd(dir, VScale(front, -1));
	}
	if (keycon.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT))
	{
		dir = VAdd(dir, VScale(left, -1));
	}
	if (keycon.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT))
	{
		dir = VAdd(dir, left);
	}
	if (dir.x != 0.0f || dir.z != 0.0f)
	{
		dir = VNorm(dir);
		transform_->pos = VAdd(transform_->pos, VScale(VNorm(dir), moveSpeed));
		transform_->rot = CalcRot(dir);
		if (isDash)
		{
			SetAnimation("Dash", false);
		}
		else
		{
			if (model_->IsPlayAnim("Walk"))
			{
				SetAnimation("Walk", false);
			}
			else
			{
				SetAnimation("Walk", true);
			}
		}
		return;
	}

	//PADのスティック情報を取得
	auto stick2D = (keycon.GetKnockLStickSize(KeyConfig::JOYPAD_NO::PAD1));
	if (stick2D.x == 0.0f && stick2D.y == 0.0f)
	{
		if (!model_->IsAnimIdle())
		{
			SetAnimation("Idle", true);
		}
		return;
	}
	SetAnimation("Walk", false);
	//スティック情報を3D情報に変更
	auto stick3D = Utility::Normalize(stick2D);
	auto moves = VAdd(VScale(front, stick3D.y * moveSpeed * -1), VScale(left, stick3D.x * moveSpeed * -1));
	transform_->rot = CalcRot(moves);
	//座標を更新する
	transform_->pos = VAdd(transform_->pos,moves );

}

VECTOR Player::CalcRot(VECTOR dir)
{
	return VECTOR(0,atan2f(dir.x,dir.z),0);
}

VECTOR Player::CalcRot(IntVector2 dir)
{
	return VECTOR(0, atan2f(dir.x, -dir.y) , 0);
}

void Player::CalcHeadPos(void)
{
	//高さ計算
	float headHeight = blockSize_ * params_.HEAD_HEIGHT;
	//足元座標から上方向に加算
	headTrans_->pos = VAdd(transform_->pos, VScale(Utility::DIR_U, headHeight));
}

void Player::InitCollider(void)
{
	Collider::TAG tag = Collider::TAG::PLAYER;
	std::vector<Collider::TAG> notHitTags;
	notHitTags.push_back(Collider::TAG::PLAYER);
	notHitTags.push_back(Collider::TAG::PLAYER_ATTACK_SWORD);
	notHitTags.push_back(Collider::TAG::PLAYER_ATTACK_ARROW);
	std::unique_ptr<Capsule> geo = std::make_unique<Capsule>(transform_->pos, headTrans_->pos, params_.COLLISION_SIZE.x * blockSize_);
	MakeCollider(tag, std::move(geo), notHitTags);
}
