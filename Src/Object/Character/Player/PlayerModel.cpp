#include <fstream>
#include "../../../Application.h"
#include "../../../Utility/Utility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Resource/JsonResource.h"
#include "../../../Manager/Resource/ShaderResource.h"
#include "../../Vertex/VertexInfo.h"
#include "Skin/SkinManager.h"
#include "Skin/Skin.h"
#include "PlayerModel.h"

PlayerModel::PlayerModel(std::string skinName, nlohmann::json& params) : params_(params)
{
	LoadModelInfo();
	SetRendererInfo();
	MakePokygonInfo();
	LoadSkin(skinName);
}

PlayerModel::~PlayerModel(void)
{
}

void PlayerModel::Init(void)
{
}

void PlayerModel::Update(void)
{
}

void PlayerModel::Draw(void)
{
	renderer_->Draw();
	//DrawGraph(0, 0, skinHandle_, true);
}

void PlayerModel::UIDraw(void)
{
}

void PlayerModel::SetSkinHandle(int handle)
{
	skinHandle_ = handle;
}

void PlayerModel::LoadModelInfo(void)
{
	pixelNum_ = params_["PixelNum"];
	auto& resourceManager = ResourceManager::GetInstance();
	auto jsonResource = resourceManager.GetJsonResource(params_["Model"].get<std::string>()).lock();
	modelInfo_ = jsonResource->GetData();
	auto& textureSize = modelInfo_["TextureSize"];
	textureSize_ = Vector2F(textureSize["x"], textureSize["y"]);
	shaderInfo_.VSKey = modelInfo_["Shader"]["VS"];
	shaderInfo_.PSKey = modelInfo_["Shader"]["PS"];
	for (auto& param : modelInfo_["Parts"])
	{
		Model_Part part;
		part.name = param["Name"];
		//if (part.name != "RightArm")
		//{
		//	continue;
		//}
		part.parentName = param["Parent"];
		part.affectParent.isRot = param["Affect"]["IsRot"];
		part.affectParent.isPos = param["Affect"]["IsPos"];
		auto& pivot = param["Pivot"];
		part.pivot = IntVector3{ pivot["x"], pivot["y"], pivot["z"] };
		for (auto& cubes : param["Cubes"])
		{
			Cube cube;
			auto& offset = cubes["Offset"];
			cube.offset = IntVector3{ offset["x"], offset["y"], offset["z"] };
			auto& pixelSize = cubes["PixelSize"];
			cube.pixelSize = IntVector3{ pixelSize["x"], pixelSize["y"], pixelSize["z"] };
			auto& uvOffset = cubes["UV"];
			cube.uvOffset = Vector2F(uvOffset["x"] / textureSize_.u, uvOffset["y"] / textureSize_.v);
			part.cubes.push_back(cube);
		}
		modelParts_.push_back(part);
	}
}

void PlayerModel::LoadSkin(std::string skinName)
{
	auto& skinManager = SkinManager::GetInstance();
	skinHandle_ = skinManager.GetHandleId(skinName);
	material_->AddTextureBuf(skinHandle_);
}

void PlayerModel::MakePokygonInfo(void)
{
	polygonInfo_.clear();
	for (auto& part : modelParts_)
	{
		for (auto& cube : part.cubes)
		{
			VertexInfo::Cube_Param param;
			param.key = modelInfo_["RenderTemplate"].get<std::string>();
			param.center = (part.pivot + cube.offset).ToVECTOR();
			param.cubeSize = cube.pixelSize.ToVECTOR();
			param.TextureSize = textureSize_;
			param.startUV = cube.uvOffset;
			auto cubePolygonInfo = VertexInfo::LoadFromFile(param);
			int indexOffset = static_cast<int>(polygonInfo_.vertex.size());
			polygonInfo_.vertex.insert(polygonInfo_.vertex.end(), cubePolygonInfo.vertex.begin(), cubePolygonInfo.vertex.end());
			for (auto& index : cubePolygonInfo.Indices)
			{
				polygonInfo_.Indices.push_back(index + indexOffset);
			}
		}
	}
}

void PlayerModel::SetRendererInfo(void)
{
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	auto shaderVSResource = resourceManager.GetShaderResource(shaderInfo_.VSKey).lock();
	auto shaderPSResource = resourceManager.GetShaderResource(shaderInfo_.PSKey).lock();
	material_ = std::make_unique<Polygon3DMaterial>(shaderVSResource->GetHandleId(), 1, shaderPSResource->GetHandleId(), 1);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
	material_->AddConstBufPS(FLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}
