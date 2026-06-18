#pragma once
#include <map>
#include "../ObjectBase.h"
#include "../../Common/vector.h"
#include "../../Renderer/Polygon3DRenderer.h"

class BlockInfo;

class Stage : public ObjectBase
{
public:
	Stage(void);
	~Stage(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UIDraw(void) override;
	VECTOR GetPlayerSpawnPos(void);
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
		int chanckBlockNum = -1;	//１チャンクのブロックのサイズ
		IntVector3 playerSpawnMapPos = {};
		IntVector3 enemySpawnMapPos = {};
	};

	// 黒曜石柱の情報
	struct PillarData
	{
		IntVector3 pos;	// 中心座標
		int height;		// 柱の高さ
		bool hasCage;	// 鉄格子付きか
	};

	//ステージのブロックのデータ
	//struct MapBlockData
	//{
	//	IntVector3 mapPos;	//マップ座標
	//	unsigned short blockId;	//ブロックID
	//};

	struct FaceCheck
	{
		IntVector3 offSet = {};
		uint8_t flag = 0;
	};


	std::vector<PillarData> pillars_;

	JsonInput jsonInput_;

	//プレイヤースポーンポイント
	IntVector3 playerSpawnPoint_;

	//エネミースポーンポイント
	IntVector3 enemySpawnPoint_;

	//ブロックのidと名前の対応表
	std::map<std::string, unsigned short> listNameAndID_;

	//ステージデータ (座標とブロックの種類(ID))
	//std::unordered_map<IntVector3, unsigned short> stageData_;

	//ステージデータチャンク毎
	//std::map<IntVector3, std::vector<MapBlockData>> chankDatas_;
	std::map<IntVector3, std::unordered_map<IntVector3,unsigned short>> chankDatas_;

	//隣にブロックがあるか判定するときに使うもの
	std::map<std::string,FaceCheck> faceChackList_;

	//ブロックの情報
	std::unique_ptr<BlockInfo> blockInfo_;

	//std::string SelectStageFilePath(void);
	//void LoadStageData(const std::string filePath);

	//チェック用のものを初期化
	void InitCheckList(void);

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
	//プレイヤーのスポーン地点を作る
	void CreatePlayerSpawnPoint(void);

	//ブロックを設置する
	void SetBlock(const IntVector3& pos, std::string blockName);
	void SetBlock(const IntVector3& pos, unsigned short blockId);
	//ブロックを削除する
	void DeleteBlock(const IntVector3& pos);

	//ポリゴン情報を更新する
	void UpdatePolygon(void);

	IntVector3 WorldPosToMapPos(IntVector3 worldPos)const;
	IntVector3 MapPosToChankPos(IntVector3 mapPos) const;
};