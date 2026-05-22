#pragma once
#include "Resource.h"
class ImageResource :  public Resource
{
public:
    // コンストラクタ
    ImageResource(void);
    ImageResource(nlohmann::json json);

	// デストラクタ
	~ImageResource(void)override;

    // ロード
	bool Load(void) override;

private:

	int handleId_; // 画像のハンドルID

    // 画像の情報をロード
	void LoadResourceInfo(void) override;
};

