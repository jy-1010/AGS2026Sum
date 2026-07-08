#pragma once
#include <memory>
#include <string>
#include "../../../../../../Lib/nlohmann/json.hpp"
#include "../../../../../Common/Transform.h"

class DragonActionUpdateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dragonTrans">ドラゴンのトランスフォーム</param>
	DragonActionUpdateBase(std::weak_ptr<Transform>dragonTrans,nlohmann::json json);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~DragonActionUpdateBase(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void) = 0;

protected:

	//ドラゴンのトランスフォーム
	std::weak_ptr<Transform>dragonTrans_;

	// アクション別のJSON
	nlohmann::json json_;

};