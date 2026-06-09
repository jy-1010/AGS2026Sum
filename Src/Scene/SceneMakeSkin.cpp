#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/SkinEdit/ColorPicker.h"
#include "../Object/SkinEdit/SkinCanvas.h"
#include "../Object/SkinEdit/SkinRenderer.h"
#include "../Object/SkinEdit/Undo.h"
#include "../Object/SkinEdit/PaintTool.h"
#include "../Object/SkinEdit/FloodFill.h"
#include "../Object/SkinEdit/SkinSave.h"
#include "../Object/SkinEdit/QuickPalette.h"
#include "../Object/SkinEdit/HSVRing.h"
#include "../Object/SkinEdit/SVArea.h"
#include "SceneMakeSkin.h"

SceneMakeSkin::SceneMakeSkin(void)
{
	previewPlayer_ = std::make_unique<Player>("");
	previewScreen_ = MakeScreen(Application::SCREEN_HALF_X / 2, Application::SCREEN_SIZE_Y, true);
	canvasScreen_ = MakeScreen(SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE, SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE);
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW_ROTATION);
	//camera.SetPos(VGet(0.0f, 300.0f, 100.0f));
	camera.SetTargetPos(VGet(0.0f, 100.0f, 0.0f));
	camera.SetFollow(previewPlayer_->GetTransform().lock());
	colorPicker_ = std::make_shared<ColorPicker>();
	skinCanvas_ = std::make_shared<SkinCanvas>();
	undo_ = std::make_shared<Undo>();
	paintTool_ = std::make_shared<PaintTool>();
	quickPalette_ = std::make_shared<QuickPalette>();
}

SceneMakeSkin::~SceneMakeSkin(void)
{
}

bool SceneMakeSkin::Init(void)
{
	offset_.x = (Application::SCREEN_SIZE_X - SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE) / 2;

	offset_.y = (Application::SCREEN_SIZE_Y - SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE) / 2;

	int uiX =
		offset_.x +
		SkinCanvas::SIZE *
		SkinRenderer::PIXEL_SIZE +
		UI_OFFSET_X;

	int uiY =
		offset_.y;

	quickPalette_->SetPosition(
		uiX,
		uiY);

	colorPicker_->UpdatePosition(
		offset_.x,
		offset_.y);

	//hsvRing_->SetPosition(
	//	900,
	//	552);

	//svArea_->SetPosition(
	//	860,
	//	552);

	return true;
}

void SceneMakeSkin::Update(void)
{
	// 更新
	previewPlayer_->Update();
	colorPicker_->Update();
	quickPalette_->Update();

	//svArea_.Update();

	if (quickPalette_->IsSelected())
	{
		colorPicker_->SetColor(
			quickPalette_->GetSelectedColor());
	}

	// 入力取得
	char keys[256];
	GetHitKeyStateAll(keys);

	int mouseX;
	int mouseY;

	GetMousePoint(&mouseX, &mouseY);

	static bool prevClick = false;
	static bool prevZ = false;

	bool nowClick =
		(GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

	bool clickDown =
		nowClick && !prevClick;

	// ペンとバケツ切り替え
	static bool prevF = false;

	bool nowF =
		CheckHitKey(KEY_INPUT_F) != 0;

	if (nowF && !prevF)
	{
		paintTool_->Toggle();
	}

	prevF = nowF;

	// スキン座標変換
	int x =
		(mouseX - offset_.x) /
		SkinRenderer::PIXEL_SIZE;

	int y =
		(mouseY - offset_.y) /
		SkinRenderer::PIXEL_SIZE;

	// Undo保存
	if (paintTool_->IsPen())
	{
		if (clickDown)
		{
			undo_->Push(
				skinCanvas_->GetData());
		}

		if (nowClick)
		{
			skinCanvas_->SetPixel(
				x,
				y,
				colorPicker_->GetCurrentColor());
		}
	}

	// スポイト
	if (GetMouseInput() & MOUSE_INPUT_RIGHT)
	{
		if (x >= 0 &&
			x < SkinCanvas::SIZE &&
			y >= 0 &&
			y < SkinCanvas::SIZE)
		{
			colorPicker_->SetColor(
				skinCanvas_->GetPixel(x, y));
		}
	}

	// ペン
	if (paintTool_->IsPen())
	{
		if (nowClick)
		{
			skinCanvas_->SetPixel(
				x,
				y,
				colorPicker_->GetCurrentColor());
		}
	}

	// バケツ
	if (paintTool_->IsBucket())
	{
		if (clickDown)
		{
			undo_->Push(
				skinCanvas_->GetData());

			FloodFill::Fill(
				skinCanvas_->GetData(),
				x,
				y,
				skinCanvas_->GetPixel(x, y),
				colorPicker_->GetCurrentColor());
		}
	}

	// Ctrl + Z
	bool nowZ =
		keys[KEY_INPUT_Z] != 0;

	bool ctrl =
		keys[KEY_INPUT_LCONTROL] ||
		keys[KEY_INPUT_RCONTROL];

	if (ctrl && nowZ && !prevZ)
	{
		undo_->Pop(
			skinCanvas_->GetData());
	}

	// 保存
	static bool prevShift = false;

	bool nowShift =
		CheckHitKey(KEY_INPUT_RSHIFT) != 0;

	if (nowShift && !prevShift)
	{
		SkinSave::Save(
			skinCanvas_->GetData(),
			"skin.png");
	}

	prevShift = nowShift;

	// 前フレーム保存
	prevClick = nowClick;
	prevZ = nowZ;
}

void SceneMakeSkin::Draw(void)
{
	colorPicker_->Draw();
	SetDrawScreen(canvasScreen_);
	SkinRenderer::Draw(*skinCanvas_);

	SkinRenderer::DrawGrid(SkinRenderer::PIXEL_SIZE, SkinRenderer::GRID_COLOR);

	quickPalette_->Draw();

	//hsvRing_.Draw();
	//svArea_.Draw();

	SetDrawScreen(previewScreen_);
	ClearDrawScreen();
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.GetCamera().CameraSetting();
	previewPlayer_->SetSkinHandle(canvasScreen_);
	previewPlayer_->Draw();
	SetDrawScreen(sceneManager.GetMainScreen());
	DrawGraph(0, 0, previewScreen_,true);
	DrawGraph(offset_.x, offset_.y, canvasScreen_,true);
}

void SceneMakeSkin::Load(void)
{
}

void SceneMakeSkin::SaveSkin(void)
{
}
