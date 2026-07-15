#include<DxLib.h>
#include"../Application.h"
#include "../Utility/Utility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
#include"../Manager/Camera.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Enemy/Dragon/Dragon.h"
#include "../Object/Character/Player/Skin/SkinManager.h"
#include "../Object/Item/Block/BlockInfo.h"
#include"SceneGame.h"

SceneGame::SceneGame(void)
{
	stage_ = std::make_unique<Stage>();
	player_ = std::make_unique<Player>(SkinManager::GetInstance().GetSelectedSkinName(), stage_->GetBlockSize());
	player_->SetPos(stage_->GetPlayerSpawnPos());
	dragon_ = std::make_unique<Dragon>(*player_,stage_->GetBlockSize());
	dragon_->SetPos(stage_->GetEnemySpawnPos());
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);
	camera.SetFollow(player_->GetTransform().lock());
}

SceneGame::~SceneGame(void)
{

}


//初期化処理(初回の1度のみ実行される)
bool SceneGame::Init(void)
{
	SceneBase::Init();
	
	return true;
}

//更新処理
void SceneGame::Update(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	player_->Update();
	stage_->Update();
	dragon_->Update();
	player_->ApplyVertex();
}

//描画処理
void SceneGame::Draw(void)
{
	player_->Draw();
	dragon_->Draw();
	stage_->Draw();

	player_->UIDraw();
	dragon_->UIDraw();
	stage_->UIDraw();
}

void SceneGame::DebugDraw(void)
{
}

float SceneGame::GetBlockSize(void)
{
	return stage_->GetBlockSize();
}

IntVector3 SceneGame::WorldPosToMapPos(const VECTOR& worldPos) const
{
	return stage_->WorldPosToMapPos(worldPos);
}

IntVector3 SceneGame::MapPosToChunkPos(const IntVector3& mapPos) const
{
	return stage_->MapPosToChunkPos(mapPos);
}

IntVector3 SceneGame::WorldPosToChunkPos(const VECTOR& worldPos) const
{
	return stage_->WorldPosToChunkPos(worldPos);
}

VECTOR SceneGame::MapPosToWorldPos(const IntVector3& mapPos) const
{
	return stage_->MapPosToWorldPos(mapPos);
}

IntVector3 SceneGame::ChunkPosToMapPos(const IntVector3& chunkPos) const
{
	return stage_->ChunkPosToMapPos(chunkPos);
}

VECTOR SceneGame::ChunkPosToWorldPos(const IntVector3& chunkPos) const
{
	return stage_->ChunkPosToWorldPos(chunkPos);
}


