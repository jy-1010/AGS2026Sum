#pragma once
#include "DragonActionUpdateBase.h"

class DragonActionUpdateFlyMove :    public DragonActionUpdateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dragonTrans">ドラゴンのトランスフォーム</param>
	/// <param name="json">行動のjson</param>
	DragonActionUpdateFlyMove(std::weak_ptr<Transform>dragonTrans, nlohmann::json json, std::weak_ptr<DragonAction>parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonActionUpdateFlyMove(void)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;

private:

	// jsonのデータをロードする
	void Load(void)override;
};

