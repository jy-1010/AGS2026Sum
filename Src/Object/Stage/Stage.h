#pragma once
#include <map>
#include "../ObjectBase.h"
#include "../../Common/vector.h"
#include "../../Renderer/Polygon3DRenderer.h"

class BlockInfo;

class Stage : public ObjectBase
{
public:
	Stage();
	~Stage();
	void Init() override;
	void Update() override;
	void Draw() override;
	void UIDraw() override;
	
private:

	//描画に必要な情報
	Polygon3DRenderer::PolygonInfo polygonInfo_;
	//ピクセルマテリアル
	std::unique_ptr<Polygon3DMaterial> material_;
	//ポリゴンレンダラー
	std::shared_ptr<Polygon3DRenderer> renderer_;

	//jsonから取得する情報
	struct JsonInput
	{
		int radius = -1;	//島の半径
		int baseHeight = -1;	//島の基準硬度
		int outHeight = -1;	//島の外側の高さ
		std::vector<int> pillarHeights;	//黒曜石の柱の高さ
		int pillarNum = -1;	//柱の数
		int pillarHasCageNum = -1;	//柱に鉄格子がついている数
	};

	// 黒曜石柱の情報
	struct PillarData
	{
		IntVector3 pos;	// 中心座標
		int height;		// 柱の高さ
		bool hasCage;	// 鉄格子付きか
	};

	std::vector<PillarData> pillars_;

	JsonInput jsonInput_;

	//プレイヤースポーンポイント
	IntVector3 playerSpawnPoint_;

	//エネミースポーンポイント
	IntVector3 enemySpawnPoint_;

	//ステージデータ (座標とブロックの種類(ID))
	std::map<IntVector3, unsigned short> stageData_;

	//ブロックの情報
	std::unique_ptr<BlockInfo> blockInfo_;

	//std::string SelectStageFilePath(void);
	//void LoadStageData(const std::string filePath);

	//jsonデータをロードする
	void LoadJsonData(void);

	//レンダラーを初期化
	void InitRenderer(void);

	//ステージを作る
	void MakeStage(void);

	//メイン島を生成
	void CreateMainIsland(void);
	//黒曜石柱を生成
	void CreateObsidianPillars(void);
	//エンドクリスタルを配置
	void CreateEndCrystals(void);
	//鉄格子生成
	void CreateIronBarCages(void);
	//ドラゴン出現位置設定
	void CreateDragonSpawnPoint();
	//プレイヤーのスポーン地点を作る
	void CreatePlayerSpawnPoint(void);

	//ブロックを設置する
	void SetBlock(IntVector3 pos, std::string blockName);

	//ポリゴン情報を更新する
	void UpdatePolygon(void);
};