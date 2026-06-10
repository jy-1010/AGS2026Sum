#pragma once
#include "SceneBase.h"
#include "../Common/Vector.h"

class Player;
class ColorPicker;
class SkinCanvas;
class Undo;
class PaintTool;
class QuickPalette;
class HSVRing;
class SVArea;
class InputName;

class SceneMakeSkin : public SceneBase
{
public:
    SceneMakeSkin(void);
	~SceneMakeSkin(void)override;
    
	bool Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Load(void)override;

private:

	static constexpr int UI_OFFSET_X = 30;

	std::shared_ptr<ColorPicker> colorPicker_;
	std::shared_ptr<SkinCanvas> skinCanvas_;
	std::shared_ptr<Undo> undo_;
	std::shared_ptr<PaintTool> paintTool_;
	std::shared_ptr<QuickPalette> quickPalette_;
	std::shared_ptr<InputName> inputName_;
	//std::shared_ptr<HSVRing> hsvRing_;
	//std::shared_ptr<SVArea> svArea_;

	std::unique_ptr<Player> previewPlayer_;
	//プレイヤーを見るためのスクリーン
	int previewScreen_;

	//キャンバス用スクリーン
	int canvasScreen_;

	//保存用スクリーン
	int saveScreen_;

	//描画位置
	Vector2I offset_;

	//保存したか
	bool isSave_;

	void SaveSkin(std::string skinName) const;
};