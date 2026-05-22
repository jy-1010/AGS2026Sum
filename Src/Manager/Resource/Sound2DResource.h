#pragma once
#include "Resource.h"
class Sound2DResource : public Resource
{
public:
	// コンストラクタ
	Sound2DResource(void);
	Sound2DResource(nlohmann::json json);
	// デストラクタ
	~Sound2DResource(void)override;
	// ロード
	bool Load(void) override;
private:
	int handleId; // 音のハンドルID
	float pitch; // ピッチの高さ
	int maxVolume; // 音量の最大値
	int maxPlayNum; // 同時再生数の最大値
	bool isLoop; // ループ再生するかどうか

	// 2D音響の情報をロード
	void LoadResourceInfo(void) override;
};

