#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

class DragonAction;

class DragonActionManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	DragonActionManager(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonActionManager(void);

	/// <summary>
	/// 初期化
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

	//アクションのリスト
	std::map<int,std::shared_ptr<DragonAction>> actions_;

	//現在のアクションのID
	int currentActionId_;	

};

