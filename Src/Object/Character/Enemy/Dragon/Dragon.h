#pragma once
#include "../../../ObjectBase.h"

class DragonModel;

class Dragon :  public ObjectBase
{
public:

	struct Param
	{
		float DEFAULT_SCALE;	//通常サイズ
		float MAX_HEALTH;	//最大体力
		float health;		//体力
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Dragon(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Dragon(void)override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void) override;

	/// <summary>
	/// UI描画
	/// </summary>
	/// <param name=""></param>
	void UIDraw(void) override;

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(VECTOR pos) { transform_->pos = pos; }

private:

	//ドラゴンの情報json 
	nlohmann::json paramsJson_;

	std::unique_ptr<DragonModel> model_;

	Param params_;

	void LoadDragonInfo(void);

};

