#pragma once
#include "Resource.h"
class ModelResource :  public Resource
{
public:
	// コンストラクタ
	ModelResource(void);
	ModelResource(nlohmann::json json);
	// デストラクタ
	~ModelResource(void)override;
	// ロード
	bool Load(void) override;

private:
	int handleId_; // モデルのハンドルID

	// 3Dモデルの情報をロード
	void LoadResourceInfo(void) override;
};

