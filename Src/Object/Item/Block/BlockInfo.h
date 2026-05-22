#pragma once
#include <string>
#include <vector>
#include <map>
#include "../../../Lib/nlohmann/json.hpp"
#include "../../../Common/Vector.h"

class BlockInfo
{
public:
	struct Param
	{
		std::string name = "";	//ブロックの名前
		unsigned short id = -1;	//ブロックのID
		float hardness = -1.0f;	//ブロックの硬さ
		float luminous = 0.0f;	//ブロックの発光度
		std::string toolType = "";	//ブロックを破壊するのに適性なツールの種類
		std::vector<std::string> toolLevel;	//ブロックを破壊するのに適性なツールのレベル
		bool isCorrectAnotherTool = false;	//別のツールでも獲得可能かどうか
		Vector2F uvOffset = Vector2F(0.0f, 0.0f);	//ブロックのテクスチャのUVオフセット
	};

	BlockInfo(void);
	~BlockInfo(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void UIDraw(void);

	const Param& GetParam(unsigned short id) { return params_.at(id); }

private:
	std::map<unsigned short,Param> params_;
	nlohmann::json blocks;
	void LoadBlockInfo(const std::string blockName);
};

