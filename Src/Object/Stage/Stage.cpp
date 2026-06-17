#include <fstream>
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource/JsonResource.h"
#include "../../Manager/Resource/ImageResource.h"
#include "../../Manager/Resource/ShaderResource.h"
#include "../Item/Block/BlockInfo.h"
#include "Stage.h"

Stage::Stage(void)
{
	//LoadStageData(SelectStageFilePath());
    blockInfo_ = std::make_unique<BlockInfo>();
    listNameAndID_ = blockInfo_->GetPairNameAndID();
    LoadJsonData();
    InitRenderer();
    MakeStage();
    UpdatePolygon();
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
}

void Stage::Update(void)
{
    UpdatePolygon();
}

void Stage::Draw(void)
{
    renderer_->Draw();
}

void Stage::UIDraw(void)
{
}

//std::string Stage::SelectStageFilePath(void)
//{
//	std::ifstream stageFile(Application::PATH_JSON + "StageFileList.json");
//	nlohmann::json stageFileList;
//	stageFile >> stageFileList;
//	std::vector<std::string> stageFilePaths = stageFileList["StageFile"].get<std::vector<std::string>>();
//	int selectNum = GetRand(static_cast<int>(stageFilePaths.size()) - 1);
//	return stageFilePaths[selectNum];
//}
//
//void Stage::LoadStageData(const std::string filePath)
//{
//	std::ifstream stage(Application::PATH_JSON + "Stage/" + filePath);
//	nlohmann::json stageData;
//	stage >> stageData;
//	playerSpawnPoint_.x = stageData["Spown"]["x"].get<int>();
//    playerSpawnPoint_.y = stageData["Spown"]["y"].get<int>();
//    playerSpawnPoint_.z = stageData["Spown"]["z"].get<int>();
//}

void Stage::LoadJsonData(void)
{
    auto& resManager = ResourceManager::GetInstance();
    auto& json = resManager.GetJsonResource("CreateStageRuleJson").lock()->GetData();
    jsonInput_.radius = json["Radius"];
    jsonInput_.baseHeight = json["BaseHeight"];
    jsonInput_.outHeight = json["OutHeight"];
    for (int i = 0; i < static_cast<int>(json["PillarHeight"].size());i++)
    {
        jsonInput_.pillarHeights.push_back(json["PillarHeight"][i]);
    }
    jsonInput_.pillarNum = json["PillarNum"];
    jsonInput_.pillarHasCageNum = json["PillarHasCageNum"];
}

void Stage::InitRenderer(void)
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    auto shaderVSResource = resourceManager.GetShaderResource(blockInfo_->GetShaderInfo().VSKey).lock();
    auto shaderPSResource = resourceManager.GetShaderResource(blockInfo_->GetShaderInfo().PSKey).lock();
    material_ = std::make_unique<Polygon3DMaterial>(shaderVSResource->GetHandleId(), 1, shaderPSResource->GetHandleId(), 1);
    material_->AddTextureBuf(blockInfo_->GetImageHandle());
    renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
}

void Stage::MakeStage(void)
{
    CreateMainIsland();
	CreateObsidianPillars();
	CreateEndCrystals();
	//CreateIronBarCages();
	CreateDragonSpawnPoint();
}

void Stage::CreateMainIsland(void)
{
    IntVector3 postemp = {};
    const unsigned short endStoneId = listNameAndID_["EndStone"];

    const int radiusSq = jsonInput_.radius * jsonInput_.radius;
    // XZ平面を走査
    for (int x = -jsonInput_.radius; x <= jsonInput_.radius; x++)
    {
        for (int z = -jsonInput_.radius; z <= jsonInput_.radius; z++)
        {
            // 中心からの距離を計算
            //float distance = sqrtf(static_cast<float>(x * x + z * z));
            int distanceSq = x * x + z * z;

            // 半径外なら生成しない
            if (distanceSq > radiusSq)
            {
                continue;
            }
            //float distance = std::sqrt(static_cast<float>(distanceSq));
            // ノイズで高さを変化させる
            //float noise = PerlinNoise(x * 0.03f, z * 0.03f);
            float rateSq = distanceSq / radiusSq;

            // この地点の地表高さ
            //int height = baseHeight + static_cast<int>(noise * 8);
            int height = jsonInput_.baseHeight -static_cast<int>(rateSq *(jsonInput_.baseHeight - jsonInput_.outHeight));

            postemp.x = x;
            postemp.z = z;
            // 下方向へブロックを配置
            for (int y = 0; y <= height; y++)
            {
                //// 下に行くほど島を細くする
                //float rate = 1.0f - static_cast<float>(y) / static_cast<float>(height);

                //float currentRadius = jsonInput_.radius * rate;

                //// 円形範囲内なら配置
                //if (distance <= currentRadius)
                //{
                //    SetBlock({ x, y, z }, "EndStone");
                //}
                postemp.y = y;
                SetBlock(postemp, endStoneId);
            }
        }
    }
}

void Stage::CreateObsidianPillars(void)
{
    // 柱を配置する円の半径
    constexpr float ringRadius = 36.0f;

    for (int i = 0; i < jsonInput_.pillarNum; i++)
    {
        // 円周上に均等配置
        float angle =
            DX_TWO_PI *
            static_cast<float>(i) /
            jsonInput_.pillarNum;

        int x =static_cast<int>(cosf(angle) * ringRadius);

        int z =static_cast<int>(sinf(angle) * ringRadius);

        // 柱ごとの高さ
        int height = 76 + i * 2;

        // 2本だけ鉄格子付き
        bool hasCage = (i == 2 || i == 7);

        // 情報保存
        pillars_.push_back({{x, 0, z}, height,hasCage});

        constexpr int pillarRadius = 2;

        // 円柱を生成
        for (int px = -pillarRadius;
            px <= pillarRadius;
            px++)
        {
            for (int pz = -pillarRadius;
                pz <= pillarRadius;
                pz++)
            {
                // 円形判定
                if (px * px + pz * pz >
                    pillarRadius * pillarRadius)
                {
                    continue;
                }

                // 高さ方向へ積み上げる
                for (int y = 0; y < height; y++)
                {
                    SetBlock({ x + px, y, z + pz },"Obsidian");
                }
            }
        }
    }
}

void Stage::CreateEndCrystals(void)
{
    // 全ての柱に配置
    for (const auto& pillar : pillars_)
    {
        //CreateEntity(EntityType::END_CRYSTAL,{pillar.pos.x, pillar.height + 1,pillar.pos.z});
    }
}

void Stage::CreateIronBarCages(void)
{
    for (const auto& pillar : pillars_)
    {
        // 鉄格子なしならスキップ
        if (!pillar.hasCage)
        {
            continue;
        }

        int baseY = pillar.height;

        // 柱頂上に鉄格子を生成
        for (int x = -3; x <= 3; x++)
        {
            for (int z = -3; z <= 3; z++)
            {
                // 外周のみ配置
                bool wall =
                    abs(x) == 3 ||
                    abs(z) == 3;

                if (!wall)
                {
                    continue;
                }

                // 高さ3ブロック
                for (int y = 0; y < 3; y++)
                {
                    SetBlock({ pillar.pos.x + x, baseY + y, pillar.pos.z + z }, "IronBar");
                }
            }
        }
    }
}

void Stage::CreateDragonSpawnPoint(void)
{
    // 中央上空に出現
    //CreateEntity(EntityType::ENDER_DRAGON, { 0, 100,0 });
}

void Stage::CreatePlayerSpawnPoint()
{
    for (int x = -2; x <= 2; x++)
    {
        for (int z = -2; z <= 2; z++)
        {
            SetBlock({ x, jsonInput_.baseHeight, z }, "Obsidian");
        }
    }
}

void Stage::SetBlock(const IntVector3& pos, std::string blockName)
{
    //unsigned short id = blockInfo_->GetParam(blockName).id;
    unsigned short id = listNameAndID_[blockName];
    //if (stageData_.contains(pos))
    //{
    //    stageData_[pos] = id;
    //    return;
    //}
    //stageData_.emplace(pos, id);
    stageData_.insert_or_assign(pos, id);
}

void Stage::SetBlock(const IntVector3& pos, unsigned short blockId)
{
    stageData_.insert_or_assign(pos, blockId);
}

void Stage::UpdatePolygon(void)
{
    polygonInfo_.clear();
    for (auto& stageData : stageData_)
    {
        const IntVector3 pos = stageData.first;
        //隣接する場所にブロックがあるかを判定しなければその面を描画するため頂点を生成する
        std::vector<std::string> faseNames; 
        if (!stageData_.contains(IntVector3(pos.x - 1, pos.y, pos.z)))
        {
            faseNames.push_back("Left");
        }
        if (!stageData_.contains(IntVector3(pos.x + 1, pos.y, pos.z)))
        {
            faseNames.push_back("Right");
        }
        if (!stageData_.contains(IntVector3(pos.x, pos.y - 1, pos.z)))
        {
            faseNames.push_back("Bottom");
        }
        if (!stageData_.contains(IntVector3(pos.x, pos.y + 1, pos.z)))
        {
            faseNames.push_back("Top");
        }
        if (!stageData_.contains(IntVector3(pos.x, pos.y, pos.z - 1)))
        {
            faseNames.push_back("Back");
        }
        if (!stageData_.contains(IntVector3(pos.x, pos.y, pos.z + 1)))
        {
            faseNames.push_back("Front");
        }
        if (faseNames.empty())
        {
            continue;
        }

        auto& param = blockInfo_->GetParam(stageData.second);
        for (auto& vertex : param.fasesPolygonInfo)
        {
            if (std::find(faseNames.begin(), faseNames.end(), vertex.first) != faseNames.end())
            {
                polygonInfo_.vertex.insert( polygonInfo_.vertex.end(),vertex.second.vertex.begin(), vertex.second.vertex.end());
                int size = static_cast<int>(polygonInfo_.vertex.size());
                for (auto& index : vertex.second.Indices)
                {
                    polygonInfo_.Indices.push_back(index + size);
                }
            }
        }
    }
}
