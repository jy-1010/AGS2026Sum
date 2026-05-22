#pragma once
#include "Resource.h"
class Sound3DResource :  public Resource
{
public:
	// コンストラクタ
	Sound3DResource(void);
	Sound3DResource(nlohmann::json json);

	// デストラクタ
	~Sound3DResource(void)override;

	// ロード
	bool Load(void) override;

private:

	int handleId; // 音のハンドルID
	float pitch; // ピッチの高さ
	int maxVolume; // 音量の最大値
	int maxPlayNum; // 同時再生数の最大値
	float radius; // 音が聞こえる範囲
	VECTOR pos; // 音の位置
	bool isLoop; // ループ再生するかどうか

	// 3D音響の情報をロード
	void LoadResourceInfo(void) override;
};

