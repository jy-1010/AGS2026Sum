#pragma once
#include "../../ObjectBase.h"

class BlockBase : public ObjectBase
{
public:
	struct Param
	{
		std::string name;	//ブロックの名前
		std::string fileName;	//ブロックの画像ファイル名
		unsigned short id;	//ブロックのID
		float hardness;	//ブロックの硬さ
		float luminous;	//ブロックの発光度
		std::string toolType;	//ブロックを破壊するのに適性なツールの種類
		std::vector<std::string> toolLevel;	//ブロックを破壊するのに適性なツールのレベル
		bool isCorrectAnotherTool;	//別のツールでも獲得可能かどうか
	};

	BlockBase(void);
	virtual ~BlockBase() = default;
	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void UIDraw(void) override;
protected:
	nlohmann::json json_;
private:
};

