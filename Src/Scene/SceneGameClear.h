//#pragma once
#include "SceneBase.h"
#include <vector>

class SceneGameClear : public SceneBase
{
public:
	//メンバー関数
	//-----------------------------------------------------------
	SceneGameClear(void);

	~SceneGameClear(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

	//ロード
	void Load(void)override;
};

