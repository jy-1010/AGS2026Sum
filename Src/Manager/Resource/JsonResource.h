#pragma once
#include "Resource.h"
class JsonResource : public Resource
{
public:
	// コンストラクタ
	JsonResource(void);
	JsonResource(nlohmann::json json);
	
	// デストラクタ
	~JsonResource(void)override;

	// ロード
	bool Load(void)override;
protected:
	nlohmann::json data; // JSONの内容
	// JSONの情報をロード
	void LoadResourceInfo(void) override;
};
