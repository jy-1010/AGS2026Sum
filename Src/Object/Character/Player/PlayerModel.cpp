#include <fstream>
#include "../../../Application.h"
#include "../../../Utility/Utility.h"
#include "../../Vertex/VertexInfo.h"
#include "PlayerModel.h"

PlayerModel::PlayerModel(std::string skinName, nlohmann::json& params) : params_(params)
{
	LoadModelInfo();
	material_ = std::make_unique<Polygon3DMaterial>("PlayerVS.hlsl", 1, "PlayerPS.hlsl", 1);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
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
}

void PlayerModel::UIDraw(void)
{
}

void PlayerModel::LoadModelInfo(void)
{
	pixelNum_ = params_["PixelNum"];
	std::ifstream modelInfo(Application::PATH_JSON + params_["Model"].get<std::string>());
	modelInfo >> modelInfo_;
	auto& textureSize = modelInfo_["TextureSize"];
	textureSize_ = Vector2F(textureSize["x"], textureSize["y"]);
	for (auto& param : modelInfo_["Parts"])
	{
		Model_Part part;
		part.name = param["Name"];
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
}

void PlayerModel::MakePokygonInfo(void)
{
	polygonInfo_.clear();
	for (auto& part : modelParts_)
	{
		for (auto& cube : part.cubes)
		{
			VertexInfo::Cube_Param param;
			param.filePath = Application::PATH_JSON + modelInfo_["RenderTemplate"].get<std::string>();
			param.center = (part.pivot + cube.offset).ToVECTOR();
			param.cubeSize = cube.pixelSize.ToVECTOR();
			param.TextureSize = textureSize_;
			param.startUV = cube.uvOffset;
			auto cubePolygonInfo = VertexInfo::LoadFromFile(param);
			polygonInfo_.vertex.insert(polygonInfo_.vertex.end(), cubePolygonInfo.vertex.begin(), cubePolygonInfo.vertex.end());
		}
	}
}
