#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Utility/JsonUtility.h"
#include "../Utility/ColorUtility.h"
#include "../Manager/Scenemanager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource/JsonResource.h"
#include "../Manager/Resource/ImageResource.h"
#include "SceneTitle.h"


SceneTitle::SceneTitle(void)
{
	auto& resManager = ResourceManager::GetInstance();
	auto titleJson = resManager.GetJsonResource("TitleJson");
	selectIndex_ = 0;
	json_ = titleJson.lock()->GetData();
	LoadJson();
}

SceneTitle::~SceneTitle(void)
{

}

//初期化処理(初回の1度のみ実行される)
bool SceneTitle::Init(void)
{
	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	auto& keyConfig = KeyConfig::GetInstance();
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		selectIndex_--;
	}
	else if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		selectIndex_++;
	}
	selectIndex_ = (selectIndex_ +static_cast<int>(buttonInfo_.size())) % static_cast<int>(buttonInfo_.size());
	for (auto& button : buttonInfo_)
	{
		if (button.selectIndex == selectIndex_)
		{
			button.state = ButtonState::SELECT;
		}
		else
		{
			button.state = ButtonState::NORMAL;
		}
	}
	if (keyConfig.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		ChangeScene();
	}
}
//描画処理
void SceneTitle::Draw(void)
{
	for (auto& logo : logoInfo_)
	{
		int x = static_cast<int>(logo.pos.x * Application::SCREEN_SIZE_X);
		int y = static_cast<int>(logo.pos.y * Application::SCREEN_SIZE_Y);
		//ロゴの描画
		DrawRotaGraph(x, y, 1.0f, 0.0f, logo.imageHandle, true);
	}
	for (auto& button : buttonInfo_)
	{
		IntVector2 center = {
			static_cast<int>(Application::SCREEN_SIZE_X * ((button.rect.left + button.rect.right) / 2)),
			static_cast<int>(Application::SCREEN_SIZE_Y * ((button.rect.top + button.rect.bottom) / 2))
		};


		//ボタンの背景を描画
		DrawOutLine(button.rect, button.state);
		//ボタンのテキスト描画
		DrawRotaGraph(center.x, center.y, 1.0f, 0.0f, button.imageHandle, true);
	}
}

void SceneTitle::Load(void)
{
	auto& resJson = json_["Resource"];
	for (int i = 0;i < resJson.size();i++)
	{
		ResourceManager::GetInstance().Load(resJson[i]);
	}
}

void SceneTitle::LoadJson(void)
{
	LoadLogo();
	LoadColor();
	LoadButton();
}

void SceneTitle::LoadLogo(void)
{
	auto& logosJson = json_["Logo"];
	//ロゴの情報を取得
	for (int i = 0; i < logosJson["List"].size();i++)
	{
		LogoInfo info;
		auto& logoJson = logosJson["List"][i];
		info.name = logoJson["Name"];
		info.imageKey = logoJson["Image"];
		info.imageHandle = ResourceManager::GetInstance().GetImageResource(info.imageKey).lock()->GetHandleId();
		info.pos = JsonUtility::GetPosTo2D(logoJson);
		logoInfo_.push_back(info);
	}
}

void SceneTitle::LoadColor(void)
{
	auto& buttonsJson = json_["Buttons"];
	auto& buttonColor = buttonsJson["Color"];
	//色情報の取得
	buttonBackColor_[ButtonState::NORMAL] = JsonUtility::GetColorFloat(buttonColor["Normal"]["Back"]);
	buttonBackColor_[ButtonState::SELECT] = JsonUtility::GetColorFloat(buttonColor["Selected"]["Back"]);
	buttonBackColor_[ButtonState::PRESS] = JsonUtility::GetColorFloat(buttonColor["Pressed"]["Back"]);
	buttonTextColor_[ButtonState::NORMAL] = JsonUtility::GetColorFloat(buttonColor["Normal"]["Text"]);
	buttonTextColor_[ButtonState::SELECT] = JsonUtility::GetColorFloat(buttonColor["Selected"]["Text"]);
	buttonTextColor_[ButtonState::PRESS] = JsonUtility::GetColorFloat(buttonColor["Pressed"]["Text"]);

	//アウトラインの色を取得
	auto& outlineColor = buttonColor["OutLine"];
	blackInfo_.color = JsonUtility::GetColorFloat(outlineColor["Black"]);
	blackInfo_.thickness = outlineColor["Black"]["Thickness"];
	lightInfo_.color = JsonUtility::GetColorFloat(outlineColor["Light"]);
	lightInfo_.thickness = outlineColor["Light"]["Thickness"];
	darkInfo_.color = JsonUtility::GetColorFloat(outlineColor["Dark"]);
	darkInfo_.thickness = outlineColor["Dark"]["Thickness"];
}

void SceneTitle::LoadButton(void)
{
	auto& buttonsJson = json_["Buttons"];
	//ボタンの情報
	for (int i = 0; i < buttonsJson["List"].size();i++)
	{
		ButtonInfo info;
		auto& buttonJson = buttonsJson["List"][i];
		info.state = ButtonState::NORMAL;
		info.name = buttonJson["Name"];
		info.imageKey = buttonJson["Image"];
		info.nextScene = buttonJson["NextScene"];
		info.selectIndex = buttonJson["SelectIndex"];
		info.imageHandle = ResourceManager::GetInstance().GetImageResource(info.imageKey).lock()->GetHandleId();
		//四角の情報
		auto& rectJson = buttonJson["Rect"];
		info.rect.left = rectJson["Left"];
		info.rect.top = rectJson["Top"];
		info.rect.right = rectJson["Right"];
		info.rect.bottom = rectJson["Bottom"];
		buttonInfo_.push_back(info);
	}
}

void SceneTitle::DrawOutLine(Rect rect, ButtonState state)
{
	//色を変換する
	int black = ColorUtility::ColorChange(blackInfo_.color);
	int light = ColorUtility::ColorChange(lightInfo_.color);
	int dark = ColorUtility::ColorChange(darkInfo_.color);
	int back = ColorUtility::ColorChange(buttonBackColor_[state]);

	//座標返還
	int left = Application::SCREEN_SIZE_X * rect.left;
	int right = Application::SCREEN_SIZE_X * rect.right;
	int top = Application::SCREEN_SIZE_Y * rect.top;
	int bottom = Application::SCREEN_SIZE_Y * rect.bottom;

	//大外の黒を描画
	DrawBoxAA(left, top, right, bottom, black, false, blackInfo_.thickness);
	//うちの明るい部分を描画
	DrawLine(left + 1, top + 1, right - 1, top + 1, light, lightInfo_.thickness);
	DrawLine(left + 1, top + 1, left + 1, bottom - 1, light, lightInfo_.thickness);
	//うちの暗い部分を描画
	DrawLine(left + 1, bottom - 1, right - 1, bottom - 1, dark, darkInfo_.thickness);
	DrawLine(right - 1, top + 1, right - 1, bottom - 1, dark, darkInfo_.thickness);
	//背景色の描画
	DrawBox(left + 2, top + 2, right - 2, bottom - 2, back, true);
}

void SceneTitle::ChangeScene(void)
{
	std::string next = "";
	for (auto& button : buttonInfo_)
	{
		if (button.selectIndex == selectIndex_)
		{
			next = button.nextScene;
		}
	}
	auto& sceneManager = SceneManager::GetInstance();
	if (next == "Game")
	{
		sceneManager.ChangeScene(SceneManager::SCENE_ID::GAME,true);
		return;
	}
	else if (next == "SkinCreate")
	{
		sceneManager.ChangeScene(SceneManager::SCENE_ID::MAKE_SKIN, true);
		return;
	}
	else if (next == "SkinSelect")
	{
		sceneManager.ChangeScene(SceneManager::SCENE_ID::SELECT_SKIN, true);
		return;
	}
	else if (next == "Setting")
	{
		//現在実装なし
		return;
		sceneManager.ChangeScene(SceneManager::SCENE_ID::SETTING, true);
		return;
	}
	else if (next == "Exit")
	{
		Application::GetInstance().ExitGame();
		return;
	}

}
