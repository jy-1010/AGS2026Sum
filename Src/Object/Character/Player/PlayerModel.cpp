
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
	auto& textureSize = params_["Model"]["TextureSize"];
	textureSize_ = FLOAT2(textureSize[0], textureSize[1]);
	for (auto& param : params_["Model"]["Parts"])
	{
		Model_Part part;
		part.name = param["Name"];
		part.parentName = param["Parent"];
		auto& pivot = param["Pivot"];
		part.pivot = Int3{ pivot[0], pivot[1], pivot[2] };
		for (auto& cubes : param["Cubes"])
		{
			Cube cube;
			auto& offset = cubes["Offset"];
			cube.offset = Int3{ offset[0], offset[1], offset[2] };
			auto& pixelSize = cubes["PixelSize"];
			cube.pixelSize = Int3{ pixelSize[0], pixelSize[1], pixelSize[2] };
			auto& uvOffset = cubes["UV"];
			cube.uvOffset = FLOAT2(uvOffset[0] / textureSize_.u, uvOffset[1] / textureSize_.v);
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

}
