#pragma once
#include <memory>
#include "../../../../../Common/Transform.h"

class DragonActionUpdateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dragonTrans">ドラゴンのトランスフォーム</param>
	DragonActionUpdateBase(std::weak_ptr<Transform>dragonTrans);

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

private:

};

