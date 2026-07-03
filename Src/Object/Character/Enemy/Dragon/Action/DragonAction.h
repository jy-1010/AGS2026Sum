#pragma once
#include <string>
#include <vector>
#include <functional>

class DragonAction
{
public:

	enum class Action
	{
		NONE = 0,
		FLY_MOVE = 1,	// 飛行移動
		FLY_BREATH_ATTACK = 2,	// 飛行ブレス攻撃
		LANDING = 3,	// 着地
		LAND_ATTACK = 4,	// 地上攻撃
		TAKE_FLIGHT = 5,	// 飛行開始
		FLY_PLAYER_ATTACK = 6,	// プレイヤーへの飛行攻撃
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	DragonAction(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonAction(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);
	
	/// <summary>
	/// UI描画処理
	/// </summary>
	/// <param name=""></param>
	void UIDraw(void);

private:

	struct NextActionInfo
	{
		int id;	//次のアクションのID
		int weight;	//次のアクションの重み(優先度)
		Action nextAction;	//次のアクションの
	};

	// アクションのID
	int id_;

	// アクション
	Action action_;

	// アクションの名前
	std::string name_;

	// アクションの持続時間
	float duration_;

	// アクションの経過時間
	float time_;

	// 次のアクションのリスト
	std::vector<NextActionInfo> nextActions_;

	// 次のアクションの重みの合計
	int sumWeight_;	

	// アクションの更新処理
	std::function<void(void)> updateFunc_;

	// 各種アクションの更新処理
	void UpdateFlyMove(void);
	void UpdateFlyBreathAttack(void);
	void UpdateLanding(void);
	void UpdateLandAttack(void);
	void UpdateTakeFlight(void);
	void UpdateFlyPlayerAttack(void);

};