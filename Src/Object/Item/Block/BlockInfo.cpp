#include <fstream>
#include "../../../Application.h"
#include "BlockInfo.h"

BlockInfo::BlockInfo(void)
{
	std::ifstream blocksFile(Application::PATH_JSON + "Blocks.json");
	blocksFile >> blocks;
	std::vector<std::string> blockNames = blocks["Names"].get<std::vector<std::string>>();
	for (auto& blockName : blockNames)
	{
		LoadBlockInfo(blockName);
	}
}

BlockInfo::~BlockInfo(void)
{
}

void BlockInfo::Init(void)
{
}

void BlockInfo::Update(void)
{
}

void BlockInfo::Draw(void)
{
}

void BlockInfo::UIDraw(void)
{
}

void BlockInfo::LoadBlockInfo(const std::string blockName)
{
	Param param;
	param.name = blockName;
	param.fileName = blocks[blockName]["FileName"].get<std::string>();
	param.id = blocks[blockName]["ID"].get<unsigned short>();
	param.hardness = blocks[blockName]["Hardness"].get<float>();
	param.luminous = blocks[blockName]["Luminous"].get<float>();
	param.toolType = blocks[blockName]["ToolType"].get<std::string>();
	param.toolLevel = blocks[blockName]["ToolLevel"].get<std::vector<std::string>>();
	param.isCorrectAnotherTool = blocks[blockName]["IsCorrectAnotherTool"].get<bool>();
	params_.emplace(param.id, param);
}
