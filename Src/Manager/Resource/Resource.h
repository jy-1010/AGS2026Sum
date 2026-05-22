#pragma once
#include<string>
#include<DxLib.h>
#include "../Lib/nlohmann/json.hpp"

class Resource
{
public:

	//３分間のミリ秒
	static constexpr int THREE_MINIT_MILI_TIME = 180000;
	//音量の最大値
	static constexpr int MAX_VOLUME = 255;

	// リソースタイプ
	enum class TYPE
	{
		NONE,	
		JSON,		// JSONファイル
		IMAGE,		// 画像
		IMAGEARRAY, // 複数画像
		MODEL,		// 3Dモデル
		SOUND_2D,	// 2D音響
		SOUND_3D,	// 3D音響
		SHADER,		// シェーダー
	};



	// コンストラクタ
	Resource(void);
	Resource(nlohmann::json json);

	//デストラクタ
	virtual ~Resource(void) = default;

	//ロード
	virtual bool Load(void) = 0;

	//タイプ
	TYPE resourceType_;

	//json情報
	nlohmann::json json_;

	//ファイルのパス
	std::string path_;

protected:
	virtual void LoadResourceInfo(void);
};