#include<DxLib.h>
#include"../Application.h"
#include "../Utility/Utility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Item/Block/BlockInfo.h"
#include"SceneGame.h"

SceneGame::SceneGame(void)
{
	stage_ = std::make_unique<Stage>();
	blockInfo_ = std::make_unique<BlockInfo>();
	player_ = std::make_unique<Player>("");
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
	
}

//描画処理
void SceneGame::Draw(void)
{

}

void SceneGame::DebugDraw(void)
{
}


