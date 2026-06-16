//#pragma once
#include "SceneBase.h"
#include <vector>

class SceneTitle: public SceneBase
{
public:

	//メンバー関数
	//-----------------------------------------------------------
	SceneTitle(void);

	~SceneTitle(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

	//ロード
	void Load(void)override;

private:

	//四角
	struct Rect
	{
		float left;
		float top;
		float right;
		float bottom;
	};

	//ボタンの状態
	enum class ButtonState
	{
		NORMAL,	//通常
		SELECT,	//選択中
		PRESS,	//決定
	};

	//ボタンの情報
	struct ButtonInfo
	{
		ButtonState state;	//状態
		std::string name;	//名前
		std::string imageKey;	//画像のキー
		std::string nextScene;	//次のシーン
		int imageHandle = -1;	//画像のハンドル
		Rect rect;			//四角の情報
		int selectIndex = -1;	//配列番号
	};

	struct LogoInfo
	{
		std::string name;
		std::string imageKey;
		int imageHandle = -1;
		FloatVector2 pos;
	};

	struct OutLineInfo
	{
		VECTOR color;	//色
		float thickness =  -1.0f;	//太さ
	};

	//ボタン背景色
	std::map<ButtonState, VECTOR> buttonBackColor_;
	//ボタンテキスト色
	std::map<ButtonState, VECTOR> buttonTextColor_;

	//ボタンの情報
	std::vector<ButtonInfo> buttonInfo_;

	//ロゴの情報
	std::vector<LogoInfo> logoInfo_;

	//アウトラインの色
	OutLineInfo blackInfo_;
	OutLineInfo lightInfo_;
	OutLineInfo darkInfo_;

	nlohmann::json json_;

	//現在選択されているもの
	int selectIndex_;

	void LoadJson(void);
	
	void LoadLogo(void);
	void LoadColor(void);
	void LoadButton(void);

	void DrawOutLine(Rect rect,ButtonState state);

	void ChangeScene(void);
};