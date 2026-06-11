#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource/ShaderResource.h"
#include "../Manager/Camera.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Player/Skin/SkinManager.h"
#include "../Object/SkinEdit/ColorPicker.h"
#include "../Object/SkinEdit/SkinCanvas.h"
#include "../Object/SkinEdit/SkinRenderer.h"
#include "../Object/SkinEdit/Undo.h"
#include "../Object/SkinEdit/PaintTool.h"
#include "../Object/SkinEdit/FloodFill.h"
#include "../Object/SkinEdit/QuickPalette.h"
#include "../Object/SkinEdit/HSVRing.h"
#include "../Object/SkinEdit/SVArea.h"
#include "../Object/SkinEdit/InputName.h"
#include "SceneMakeSkin.h"

SceneMakeSkin::SceneMakeSkin(void)
{
	previewPlayer_ = std::make_unique<Player>("");
	previewScreen_ = MakeScreen(Application::SCREEN_HALF_X / 2, Application::SCREEN_SIZE_Y, true);
	canvasScreen_ = MakeScreen(SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE, SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE);
	canvasShaderScreen_ = MakeScreen(SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE, SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE);
	saveScreen_ = MakeScreen(SkinCanvas::SIZE, SkinCanvas::SIZE);
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW_ROTATION);
	camera.SetFollow(previewPlayer_->GetTransform().lock());
	colorPicker_ = std::make_shared<ColorPicker>();
	skinCanvas_ = std::make_shared<SkinCanvas>();
	undo_ = std::make_shared<Undo>();
	paintTool_ = std::make_shared<PaintTool>();
	quickPalette_ = std::make_shared<QuickPalette>();
	inputName_ = std::make_shared<InputName>();
	isSave_ = false;
	InitRenderer();
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
	auto& keyConfig = KeyConfig::GetInstance();

	if (inputName_->IsUpdate())
	{
		inputName_->Update();
		return;
	}
	if (inputName_->IsEnd())
	{
		if (isSave_)
		{
			return;
		}
		SaveSkin(inputName_->GetInputData());
		isSave_ = true;
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
		return;
	}
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

	bool inCanvas =
		x >= 0 &&
		x < SkinCanvas::SIZE &&
		y >= 0 &&
		y < SkinCanvas::SIZE;

	// Undo保存
	if (paintTool_->IsPen())
	{
		if (clickDown && inCanvas)
		{
			undo_->Push(
				skinCanvas_->GetData());
		}

		if (nowClick && inCanvas)
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
	//if (paintTool_->IsPen())
	//{
	//	if (nowClick)
	//	{
	//		skinCanvas_->SetPixel(
	//			x,
	//			y,
	//			colorPicker_->GetCurrentColor());
	//	}
	//}

	// バケツ
	if (paintTool_->IsBucket())
	{
		if (clickDown && inCanvas)
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

	// 前フレーム保存
	prevClick = nowClick;
	prevZ = nowZ;

	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		inputName_->SetFase(InputName::Fase::FIRST_CHECK);
	}
}

void SceneMakeSkin::Draw(void)
{
	colorPicker_->Draw();
	quickPalette_->Draw();
	SetDrawScreen(canvasScreen_);
	ClearDrawScreen();
	SkinRenderer::Draw(*skinCanvas_);

	SetDrawScreen(saveScreen_);
	ClearDrawScreen();
	DrawExtendGraph(0, 0, SkinCanvas::SIZE, SkinCanvas::SIZE, canvasScreen_, false);

	SetDrawScreen(canvasShaderScreen_);
	ClearDrawScreen();
	canvasMaterial_->SetTextureBuf(1, canvasScreen_);
	canvasRenderer_->Draw();
	//hsvRing_.Draw();
	//svArea_.Draw();


	SetDrawScreen(previewScreen_);
	ClearDrawScreen();
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.GetCamera().CameraSetting();
	previewPlayer_->SetSkinHandle(canvasScreen_);
	previewPlayer_->Draw();

	SetDrawScreen(canvasShaderScreen_);
	SkinRenderer::DrawGrid(SkinRenderer::PIXEL_SIZE, SkinRenderer::GRID_COLOR);

	SetDrawScreen(sceneManager.GetMainScreen());
	DrawGraph(0, 0, previewScreen_,true);
	DrawGraph(offset_.x, offset_.y, canvasShaderScreen_,true);

	DrawString(offset_.x * 2, offset_.y / 4, "右クリック：スポイト", GetColor(255, 255, 255));

	if (paintTool_->IsPen())
	{
		DrawString(
			offset_.x,
			offset_.y / 4,
			"Mode : PEN (Fで切替)",
			GetColor(255, 255, 255));
	}

	if (paintTool_->IsBucket())
	{
		DrawString(
			offset_.x,
			offset_.y / 4,
			"Mode : BUCKET (Fで切替)",
			GetColor(255, 255, 0));
	}

	inputName_->Draw();
}

void SceneMakeSkin::Load(void)
{
}

void SceneMakeSkin::SaveSkin(std::string skinName) const
{
	SaveDrawValidGraphToPNG(saveScreen_, 0, 0, SkinCanvas::SIZE, SkinCanvas::SIZE , (Application::PATH_IMAGE + "PlayerSkin/" + skinName + ".png").c_str());
	auto& skinManager = SkinManager::GetInstance();
	skinManager.SaveSkin(skinName);
}

void SceneMakeSkin::InitRenderer(void)
{
	auto& resManager = ResourceManager::GetInstance();
	int shaderHandle = resManager.GetShaderResource("CanvasPS").lock()->GetHandleId();
	canvasMaterial_ = std::make_shared<Polygon2DMaterial>(shaderHandle,0);
	auto& skinManager = SkinManager::GetInstance();
	int defaultSkin = skinManager.GetHandleId(skinManager.GetDefaultSkinName());
	canvasMaterial_->AddTextureBuf(defaultSkin);
	canvasMaterial_->AddTextureBuf(defaultSkin);
	canvasRenderer_ = std::make_shared<Polygon2DRenderer>(*canvasMaterial_, canvasPolygonInfo_);

	canvasPolygonInfo_.clear();
	auto& vertex = canvasPolygonInfo_.vertex;
	for (int i = 0; i < 4; i++)
	{
		VERTEX2DSHADER ver;
		ver.pos = VGet((i % 2) * (SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE),(i / 2) * (SkinCanvas::SIZE * SkinRenderer::PIXEL_SIZE), 0.0f);
		ver.dif = GetColorU8(255, 255, 255, 255);
		ver.rhw = 1.0f;
		ver.u = static_cast<float>(i % 2);
		ver.v = static_cast<float>(i / 2);
		ver.su = ver.pos.x / Application::SCREEN_SIZE_X;
		ver.sv = ver.pos.y / Application::SCREEN_SIZE_Y;
		ver.spc = GetColorU8(0, 0, 0, 0);
		vertex.push_back(ver);
	}
	canvasPolygonInfo_.Indices = { 0,1,2,1,3,2 };
}
