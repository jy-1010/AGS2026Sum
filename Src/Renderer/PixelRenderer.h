#pragma once
#include <DxLib.h>
#include "../Common/Vector.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/BaseRenderer.h"

class PixelRenderer : public BaseRenderer
{

public:

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	// コンストラクタ
	PixelRenderer(PixelMaterial& pixelMaterial);

	// デストラクタ
	~PixelRenderer(void)override;

	// 描画矩形の生成
	void MakeSquereVertex(Vector2I pos, Vector2I size);
	void MakeSquereVertex(void);

	// 座標設定
	void SetPos(Vector2I pos);
	// 画像サイズ設定
	void SetSize(Vector2I size);

	// 描画
	void Draw(void)override;
	void Draw(int x, int y);

private:

	// 座標
	Vector2I pos_;

	// 描画サイズ
	Vector2I size_;

	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	// ピクセルマテリアル
	PixelMaterial& pixelMaterial_;

};
