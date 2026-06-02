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

	struct AffectParent
	{
		bool isRot = false;
		bool isPos = false;
		const bool IsAffect(void)const { return isRot || isPos; }
	};

	struct ShaderInfo
	{
		std::string VSKey;	// 頂点シェーダーのリソースキー
		std::string PSKey;	// ピクセルシェーダーのリソースキー
	};

	struct Cube
	{
		IntVector3 offset;	//キューブの中心からのオフセット
		IntVector3 pixelSize;
		Vector2F uvOffset;
	};

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

	PlayerModel(std::string skinName, nlohmann::json& params);
	~PlayerModel(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void UIDraw(void);

	void SetSkinHandle(int handle);
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
	Vector2F textureSize_;

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

