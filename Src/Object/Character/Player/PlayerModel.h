#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../../../Lib/nlohmann/json.hpp"
#include "../../../Renderer/Polygon3DRenderer.h"
#include "../../../Common/Vector.h"

class PlayerAnimationManager;

class PlayerModel
{
public:

	//親の情報を引き継ぐか
	struct AffectParent
	{
		bool isRot = false;
		bool isPos = false;
		const bool IsAffect(void)const { return isRot || isPos; }
	};

	//シェーダー情報
	struct ShaderInfo
	{
		std::string VSKey;	// 頂点シェーダーのリソースキー
		std::string PSKey;	// ピクセルシェーダーのリソースキー
	};

	//キューブ情報
	struct Cube
	{
		IntVector3 offset;	//キューブの中心からのオフセット
		IntVector3 pixelSize;
		FloatVector2 uvOffset;
	};

	//モデルのパーツごとの情報
	struct Model_Part
	{
		std::string name;
		std::string parentName;
		AffectParent affectParent;
		IntVector3 pivot;	//回転中心
		std::vector<Cube> cubes;
	};

	struct ModelInfo
	{
		Model_Part part;
		Polygon3DRenderer::PolygonInfo polygonInfo;
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="skinName">今回使用するスキンの名前</param>
	/// <param name="params">プレイヤーの情報json</param>
	PlayerModel(std::string skinName, nlohmann::json& params);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~PlayerModel(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// UIの描画処理
	/// </summary>
	/// <param name=""></param>
	void UIDraw(void);

	/// <summary>
	/// スキンのハンドルを適用
	/// </summary>
	/// <param name="handle">ハンドルID</param>
	void SetSkinHandle(int handle);

	/// <summary>
	/// アニメーションを割り当てる
	/// </summary>
	/// <param name="animName">アニメーションの名前</param>
	/// <param name="isCompulsion">強制的に変えるか</param>
	void SetAnimation(std::string animName, bool isCompulsion);

private:
	//アニメーションマネージャー
	std::unique_ptr<PlayerAnimationManager> animationManager_;

	//アニメーションのkey
	std::string animationKey_;

	//ピクセル数
	int pixelNum_;

	//スキンのハンドル
	int skinHandle_;

	//プレイヤーモデルのパラメータ
	nlohmann::json& params_;

	//モデルの情報
	nlohmann::json modelInfo_;

	//テクスチャのサイズ
	FloatVector2 textureSize_;

	//モデルの大親の名前
	std::string rootPartName_;

	//モデルのパーツの情報
	std::vector <Model_Part> modelParts_;

	//描画に必要な情報
	Polygon3DRenderer::PolygonInfo polygonInfo_;

	//パーツの名前とモデルの情報のマップ
	std::vector<ModelInfo> modelInfos_;

	//ピクセルマテリアル
	std::unique_ptr<Polygon3DMaterial> material_;
	//ポリゴンレンダラー
	std::shared_ptr<Polygon3DRenderer> renderer_;

	//シェーダの情報
	ShaderInfo shaderInfo_;

	//モデルの情報を読み込む
	void LoadModelInfo(void);

	//スキンを読み込む
	void LoadSkin(std::string skinName);

	//ポリゴンの情報を作成する
	void MakePokygonInfo(void);

	//シェーダの情報を設定する
	void SetRendererInfo(void);
};

