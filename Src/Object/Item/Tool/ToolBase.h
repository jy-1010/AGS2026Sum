#pragma once
#include "../../ObjectBase.h"
class ToolBase : public ObjectBase
{
public:
	struct Param
	{
		std::string type;	//ツールの種類
		std::string level;	//ツールのレベル
		float durability;	//ツールの耐久値
	};
};

