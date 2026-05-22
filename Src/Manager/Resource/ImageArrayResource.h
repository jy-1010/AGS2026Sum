#pragma once
#include "Resource.h"
class ImageArrayResource :  public Resource
{
public:
	// コンストラクタ
	ImageArrayResource(void);
	ImageArrayResource(nlohmann::json json);

	// デストラクタ
	~ImageArrayResource(void)override;

	// ロード
	bool Load(void) override;
private:
	std::vector<int> handleIds; // 複数画像のハンドルID配列
	int numX; // 画像の横方向の分割数
	int numY; // 画像の縦方向の分割数
	int sizeX; // 分割された1枚あたりの幅
	int sizeY; // 分割された1枚あたりの高さ

	// 画像の情報をロード
	void LoadResourceInfo(void) override;
};

