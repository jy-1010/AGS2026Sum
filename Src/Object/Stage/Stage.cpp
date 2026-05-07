#include <fstream>
#include "../../Application.h"
#include "Stage.h"

Stage::Stage(void)
{
	LoadStageData(SelectStageFilePath());
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
}

void Stage::UIDraw(void)
{
}

std::string Stage::SelectStageFilePath(void)
{
	std::ifstream stageFile(Application::PATH_JSON + "StageFileList.json");
	nlohmann::json stageFileList;
	stageFile >> stageFileList;
	std::vector<std::string> stageFilePaths = stageFileList["StageFile"].get<std::vector<std::string>>();
	int selectNum = GetRand(static_cast<int>(stageFilePaths.size()) - 1);
	return stageFilePaths[selectNum];
}

void Stage::LoadStageData(const std::string filePath)
{
	std::ifstream stage(Application::PATH_JSON + "Stage/" + filePath);
	nlohmann::json stageData;
	stage >> stageData;
	spawnPoint_.x = stageData["Spown"]["x"].get<int>();
	spawnPoint_.y = stageData["Spown"]["y"].get<int>();
	spawnPoint_.z = stageData["Spown"]["z"].get<int>();
}
