#pragma once
#include "SceneBase.h"
class SceneMenu :   public SceneBase
{
public:

	SceneMenu(void);
	~SceneMenu(void)override;

	bool Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Load(void)override;
private:
};

