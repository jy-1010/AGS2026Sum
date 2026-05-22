#pragma once
#include "Resource.h"
class ShaderResource : public Resource
{
public:
	// コンストラクタ
	ShaderResource(void);
	ShaderResource(nlohmann::json json);
	// デストラクタ
	~ShaderResource(void)override;
	// ロード
	bool Load(void) override;

private:
	int handleId_; // シェーダーのハンドルID
	// シェーダーの情報をロード
	void LoadResourceInfo(void) override;
};

