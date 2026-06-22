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
	maxLayerNum_ = -1;
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
	for (int i = 0; i <= maxLayerNum_; i++)
	{
		auto logos = GetLogoInfoToLayer(i);
		for (auto& logo : logos)
		{
			int x = static_cast<int>(logo.pos.x * Application::SCREEN_SIZE_X);
			int y = static_cast<int>(logo.pos.y * Application::SCREEN_SIZE_Y);
			//ロゴの描画
			DrawRotaGraph(x, y, 1.0f, 0.0f, logo.imageHandle, true);
		}
		auto buttons = GetButtonInfoToLayer(i);
		for (auto& button : buttons)
		{
			DrawButton(button);
		}
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
		info.layer = logoJson["Layer"];
		maxLayerNum_ = maxLayerNum_ < info.layer ? info.layer : maxLayerNum_;
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
		info.layer = buttonJson["Layer"];
		maxLayerNum_ = maxLayerNum_ < info.layer ? info.layer : maxLayerNum_;
		info.imageHandle = ResourceManager::GetInstance().GetImageResource(info.imageKey).lock()->GetHandleId();
		//四角の情報
		auto& rectJson = buttonJson["Rect"];
		info.rect.leftPer = rectJson["Left"];
		info.rect.topPer = rectJson["Top"];
		info.rect.rightPer = rectJson["Right"];
		info.rect.bottomPer = rectJson["Bottom"];
		info.rect.leftScreen = Application::SCREEN_SIZE_X * info.rect.leftPer;
		info.rect.rightScreen = Application::SCREEN_SIZE_X * info.rect.rightPer;
		info.rect.topScreen = Application::SCREEN_SIZE_Y * info.rect.topPer;
		info.rect.bottomScreen = Application::SCREEN_SIZE_Y * info.rect.bottomPer;
		buttonInfo_.push_back(info);
	}
}

std::vector<SceneTitle::ButtonInfo> SceneTitle::GetButtonInfoToLayer(int layerNum)
{
	std::vector<ButtonInfo>ret;
	for (auto& button : buttonInfo_)
	{
		if (button.layer = layerNum)
		{
			ret.push_back(button);
		}
	}
	return ret;
}

std::vector<SceneTitle::LogoInfo> SceneTitle::GetLogoInfoToLayer(int layerNum)
{
	std::vector<LogoInfo>ret;
	for (auto& logo : logoInfo_)
	{
		if (logo.layer = layerNum)
		{
			ret.push_back(logo);
		}
	}
	return ret;
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

void SceneTitle::DrawButton(ButtonInfo button)
{
	IntVector2 center = {
		static_cast<int>(((button.rect.leftScreen + button.rect.rightScreen) / 2)),
		static_cast<int>(((button.rect.topScreen + button.rect.bottomScreen) / 2))
	};
	DrawOutLine(button.rect, button.state);
	//ボタンのテキスト描画
	DrawRotaGraph(center.x, center.y, 1.0f, 0.0f, button.imageHandle, true);
}

void SceneTitle::DrawOutLine(Rect rect, ButtonState state)
{
	//色を変換する
	int black = ColorUtility::ColorChange(blackInfo_.color);
	int light = ColorUtility::ColorChange(lightInfo_.color);
	int dark = ColorUtility::ColorChange(darkInfo_.color);
	int back = ColorUtility::ColorChange(buttonBackColor_[state]);

	//座標返還

	//大外の黒を描画
	DrawBoxAA(rect.leftScreen, rect.topScreen, rect.rightScreen, rect.bottomScreen, black, false, blackInfo_.thickness);
	//うちの明るい部分を描画
	DrawLine(rect.leftScreen + 1, rect.topScreen + 1, rect.rightScreen - 1, rect.topScreen + 1, light, lightInfo_.thickness);
	DrawLine(rect.leftScreen + 1, rect.topScreen + 1, rect.leftScreen + 1, rect.bottomScreen - 1, light, lightInfo_.thickness);
	//うちの暗い部分を描画
	DrawLine(rect.leftScreen + 1, rect.bottomScreen - 1, rect.rightScreen - 1, rect.bottomScreen - 1, dark, darkInfo_.thickness);
	DrawLine(rect.rightScreen - 1, rect.topScreen + 1, rect.rightScreen - 1, rect.bottomScreen - 1, dark, darkInfo_.thickness);
	//背景色の描画
	DrawBox(rect.leftScreen + 2, rect.topScreen + 2, rect.rightScreen - 2, rect.bottomScreen - 2, back, true);
}
