#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Utility/ColorUtility.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/SceneManager.h"
#include "InputName.h"

InputName::InputName(void)
{
	fase_ = Fase::NONE;
	screenSize_ = { Application::SCREEN_SIZE_X - MARGIN * 2, Application::SCREEN_SIZE_Y - MARGIN * 2 };
	screen_ = MakeScreen(screenSize_.x,screenSize_.y);
}

InputName::~InputName(void)
{
}

void InputName::Update(void)
{
	switch (fase_)
	{
	case Fase::NONE:
	case Fase::END:
		return;
	case Fase::FIRST_CHECK:
	case Fase::SECOND_CHECK:
		CheckUpdate();
		break;
	case Fase::INPUT:
		InputUpdate();
		break;
	}
}

void InputName::Draw(void)
{
	SetDrawScreen(screen_);
	ClearDrawScreen();
	switch (fase_)
	{
	case Fase::NONE:
	case Fase::END:
		return;
	case Fase::FIRST_CHECK:
	case Fase::SECOND_CHECK:
		CheckDraw();
		break;
	case Fase::INPUT:
		InputDraw();
		break;
	}
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());
	DrawGraph(MARGIN, MARGIN, screen_, true);
}

void InputName::CheckUpdate(void)
{
	auto& keycon = KeyConfig::GetInstance();
	if (keycon.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		fase_ = static_cast<Fase>(static_cast<int>(fase_) + 1);
	}
	if (keycon.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		fase_ = static_cast<Fase>(static_cast<int>(fase_) - 1);
	}
}

void InputName::InputUpdate(void)
{
	auto& keycon = KeyConfig::GetInstance();
	std::vector<int> pushKey;
	pushKey = keycon.GetPushBtns();
	for (auto& key : pushKey)
	{
		inputData_ += Utility::KeyCodeToString(key);
		switch (key)
		{
		case KEY_INPUT_BACK:
			if (inputData_.size() > 0)
			{
				inputData_.pop_back();
			}
			continue;
		}
	}
	if (keycon.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		fase_ = static_cast<Fase>(static_cast<int>(fase_) + 1);
	}
}

void InputName::CheckDraw(void)
{
	DrawBox(0, 0, screenSize_.x, screenSize_.y, ColorUtility::WHITE, true);
	std::string checkstr = "";
	if (fase_ == Fase::FIRST_CHECK)
	{
		checkstr += "Is this skin OK?";
	}
	else
	{
		checkstr += "Is this name OK?";
	}
	Utility::DrawStringPlace(checkstr, screenSize_.x / 2, screenSize_.y / 4, ColorUtility::BLACK, Utility::STRING_PLACE::CENTER);
	Utility::DrawStringPlace("Yes : SPACE ENTER", screenSize_.x / 4, screenSize_.y / 2, ColorUtility::BLACK, Utility::STRING_PLACE::CENTER);
	Utility::DrawStringPlace("No : BACKSPACE LCONTROL",screenSize_.x - screenSize_.x / 4, screenSize_.y / 2, ColorUtility::BLACK, Utility::STRING_PLACE::CENTER);
}

void InputName::InputDraw(void)
{
	DrawBox(0, 0, screenSize_.x, screenSize_.y, ColorUtility::WHITE, true);
	Utility::DrawStringPlace("Input Name \n" + inputData_, screenSize_.x / 4, screenSize_.y / 4, ColorUtility::BLACK, Utility::STRING_PLACE::LEFT);
}
