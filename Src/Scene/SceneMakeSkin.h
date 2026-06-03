#pragma once
#include "SceneBase.h"

class Player;

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

	std::unique_ptr<Player> previewPlayer_;
	//プレイヤーを見るためのスクリーン
	int previewScreen_;

	void SaveSkin(void);
};

