#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../../../Lib/nlohmann/json.hpp"
#include "../../../Renderer/Polygon3DRenderer.h"

class PlayerModel
{
public:

	struct Int3
	{
		int x;
		int y;
		int z;
	};

	struct Cube
	{
		Int3 offset;
		Int3 pixelSize;
		FLOAT2 uvOffset;
	};

	struct Model_Part
	{
		std::string name;
		std::string parentName;
		Int3 pivot;
		std::vector<Cube> cubes;
	};

	PlayerModel(std::string skinName, nlohmann::json& params);
	~PlayerModel(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void UIDraw(void);
private:
	int skinHandle_;
	nlohmann::json& params_;
	FLOAT2 textureSize_;
	std::vector <Model_Part> modelParts_;
	Polygon3DRenderer::PolygonInfo polygonInfo_;
	std::unique_ptr<Polygon3DMaterial> material_;
	std::shared_ptr<Polygon3DRenderer> renderer_;

	void LoadModelInfo(void);
	void LoadSkin(std::string skinName);
	void MakePokygonInfo(void);
};

