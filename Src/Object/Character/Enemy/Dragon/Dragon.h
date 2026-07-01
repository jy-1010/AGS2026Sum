#pragma once
#include "../../../ObjectBase.h"
class Dragon :  public ObjectBase
{
public:

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

};

