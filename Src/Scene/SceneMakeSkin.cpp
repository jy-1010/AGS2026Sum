#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Character/Player/Player.h"
#include "SceneMakeSkin.h"

SceneMakeSkin::SceneMakeSkin(void)
{
	previewPlayer_ = std::make_unique<Player>("");
	previewScreen_ = MakeScreen(Application::SCREEN_HALF_X / 2, Application::SCREEN_SIZE_Y, true);
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetPos(VGet(0.0f, 300.0f, 100.0f));
	camera.SetTargetPos(VGet(0.0f, 100.0f, 0.0f));
}

SceneMakeSkin::~SceneMakeSkin(void)
{
}

bool SceneMakeSkin::Init(void)
{
	return false;
}

void SceneMakeSkin::Update(void)
{
	previewPlayer_ ->Update();
}

void SceneMakeSkin::Draw(void)
{
	SetDrawScreen(previewScreen_);
	ClearDrawScreen();
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.GetCamera().CameraSetting();
	previewPlayer_->Draw();
}

void SceneMakeSkin::Load(void)
{
}

void SceneMakeSkin::SaveSkin(void)
{
}
