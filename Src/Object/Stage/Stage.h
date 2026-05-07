#pragma once
#include <map>
#include "../ObjectBase.h"
#include "../../Common/IntVector3.h"

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

	IntVector3 spawnPoint_;
	//ステージデータ (座標とブロックの種類(ID))
	std::map<IntVector3, unsigned short> stageData_;

	std::string SelectStageFilePath(void);
	void LoadStageData(const std::string filePath);
};

