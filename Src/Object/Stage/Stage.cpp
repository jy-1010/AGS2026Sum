#include <fstream>
#include "../../Application.h"
#include "../../Utility/JsonUtility.h"
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../../Manager/Resource/JsonResource.h"
#include "../../Manager/Resource/ImageResource.h"
#include "../../Manager/Resource/ShaderResource.h"
#include "../Item/Block/BlockInfo.h"
#include "../Vertex/VertexInfo.h"
#include "Stage.h"

Stage::Stage(void)
{
	//LoadStageData(SelectStageFilePath());
    blockInfo_ = std::make_unique<BlockInfo>();
    listNameAndID_ = blockInfo_->GetPairNameAndID();
    LoadJsonData();
    InitCheckList();
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
}

void Stage::Draw(void)
{
    renderer_->Draw();
}

void Stage::UIDraw(void)
{
}

VECTOR Stage::GetPlayerSpawnPos(void)
{
    VECTOR ret;
    ret = IntVector3(jsonInput_.playerSpawnMapPos * blockInfo_->GetSize()).ToVECTOR();
    return ret;
}

void Stage::InitCheckList(void)
{
    faceChackList_.emplace( "Left",FaceCheck{{-1,0,0},1 << 0 });
    faceChackList_.emplace( "Right",FaceCheck{{1,0,0},1 << 1 });
    faceChackList_.emplace( "Bottom",FaceCheck{{0,-1,0},1 << 2 });
    faceChackList_.emplace( "Top",FaceCheck{{0,1,0},1 << 3 });
    faceChackList_.emplace( "Back",FaceCheck{{0,0,-1},1 << 4 });
    faceChackList_.emplace( "Front",FaceCheck{{0,0,1},1 << 5 });
}

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
    jsonInput_.chanckBlockNum = json["ChankBlockNum"];
    jsonInput_.playerSpawnMapPos = JsonUtility::GetPosTo3D(json["SpawnPosition"]["Player"]);
    jsonInput_.enemySpawnMapPos = JsonUtility::GetPosTo3D(json["SpawnPosition"]["Enemy"]);
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
    CreatePlayerSpawnPoint();
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

void Stage::CreatePlayerSpawnPoint()
{
    IntVector3 playerSpawnPos = jsonInput_.playerSpawnMapPos;
    for (int x = -2; x <= 2; x++)
    {
        for (int z = -2; z <= 2; z++)
        {
            SetBlock(playerSpawnPos + IntVector3{x,-1,z}, "Obsidian");
            for (int y = 0; y <= 4; y++)
            {
                DeleteBlock(playerSpawnPos + IntVector3{ x,y,z });
            }
        }
    }
}

void Stage::SetBlock(const IntVector3& pos, std::string blockName)
{
    unsigned short id = listNameAndID_[blockName];

    SetBlock(pos, id);
}

void Stage::SetBlock(const IntVector3& pos, unsigned short blockId)
{
    std::pair<IntVector3, unsigned short> data;
    data.first = pos;
    data.second = blockId;

    //stageData_.insert_or_assign(pos, id);
    IntVector3 chankPos = MapPosToChankPos(pos);
    auto it = chankDatas_.find(chankPos);
    if (it != chankDatas_.end())
    {
        //既存の場合
        it->second.emplace(data);
        return;
    }

    chankDatas_.emplace(chankPos, std::unordered_map{ data });

}

void Stage::DeleteBlock(const IntVector3& pos)
{
    IntVector3 chankPos = MapPosToChankPos(pos);

    auto chunkIt = chankDatas_.find(chankPos);
    if (chunkIt == chankDatas_.end())
    {
        return;
    }

    chunkIt->second.erase(pos);
}

void Stage::UpdatePolygon(void)
{
    auto& sceneManager = SceneManager::GetInstance();
    auto& camera = sceneManager.GetCamera();
    VECTOR cameraPos = camera.GetPos();
    IntVector3 cameraChank = MapPosToChankPos(WorldPosToMapPos(IntVector3(cameraPos)));
    polygonInfo_.clear();
    for (auto& chankData : chankDatas_)
    {
        //チャンク座標
        const IntVector3 chankPos = chankData.first;
        const IntVector3 sub = chankPos - cameraChank;
        if (abs(sub.x) > 4 || abs(sub.y) > 4 || abs(sub.z) > 4)
        {
            continue;
        }
        int blockSize = blockInfo_->GetSize();
        if (!CheckCameraViewClip_Box((chankPos * blockSize).ToVECTOR(), ((chankPos + IntVector3{ 1,1,1 }) * blockSize).ToVECTOR()))
        {
            continue;
        }
        //チャンク内の情報
        const auto& inChankData = chankData.second;
        for (auto& stageData : inChankData)
        {
            //uint8_tでどこを描画するかを表す 1 = left,2 = right,4 = bottom,8 = top, 16 = back,32 = front
            uint8_t isDraw = 0;

            const IntVector3 mapPos = stageData.first;
            const VECTOR worldPos = (mapPos * jsonInput_.chanckBlockNum).ToVECTOR();
            //隣接する場所にブロックがあるかを判定しなければその面を描画するため頂点を生成する
            for (auto& chack : faceChackList_)
            {
                if (!inChankData.contains(chack.second.offSet))
                {
                    isDraw |= chack.second.flag;
                }
            }
            if (isDraw == 0)
            {
                continue;
            }
            auto& param = blockInfo_->GetParam(stageData.second);
            for (auto& chack : faceChackList_)
            {
                if (chack.second.flag & isDraw)
                {
                    continue;
                }
                int size = static_cast<int>(polygonInfo_.vertex.size());
                auto facePolygon = param.fasesPolygonInfo.find(chack.first);
                auto& vertex = facePolygon->second.vertex;
                auto& index = facePolygon->second.Indices;
                for (auto& ver: vertex)
                {
                    VERTEX3DSHADER vertemp = ver;
                    vertemp.pos = VAdd(ver.pos, worldPos);
                    polygonInfo_.vertex.push_back(vertemp);
                }
                for (auto& ind : index)
                {
                    polygonInfo_.Indices.push_back(ind + size);
                }
            }
        }
    }
}

IntVector3 Stage::WorldPosToMapPos(IntVector3 worldPos) const
{
    IntVector3 ret;
    int size = blockInfo_->GetSize();
    ret.x = static_cast<int>(std::floor(static_cast<float>(worldPos.x) / size));
    ret.y = static_cast<int>(std::floor(static_cast<float>(worldPos.y) / size));
    ret.z = static_cast<int>(std::floor(static_cast<float>(worldPos.z) / size));
    return ret;
}

IntVector3 Stage::MapPosToChankPos(IntVector3 mapPos) const
{
    IntVector3 ret;
    ret.x = static_cast<int>(std::floor(static_cast<float>(mapPos.x) / jsonInput_.chanckBlockNum));
    ret.y = static_cast<int>(std::floor(static_cast<float>(mapPos.y) / jsonInput_.chanckBlockNum));
    ret.z = static_cast<int>(std::floor(static_cast<float>(mapPos.z) / jsonInput_.chanckBlockNum));
    return ret;
}
