#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../../../Lib/nlohmann/json.hpp"
#include "../../../Renderer/Polygon3DRenderer.h"
#include "../../../Common/Vector.h"

class PlayerModel
{
public:

	struct AffectParent
	{
		bool isRot = false;
		bool isPos = false;
		const bool IsAffect(void)const { return isRot || isPos; }
	};

	struct Cube
	{
		IntVector3 offset;	//キューブの中心からのオフセット
		IntVector3 pixelSize;
		Vector2F uvOffset;
	};

	struct Model_Part
	{
		std::string name;
		std::string parentName;
		AffectParent affectParent;
		IntVector3 pivot;	//回転中心
		std::vector<Cube> cubes;
	};

	PlayerModel(std::string skinName, nlohmann::json& params);
	~PlayerModel(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void UIDraw(void);
private:
	int pixelNum_;
	int skinHandle_;
	nlohmann::json& params_;
	nlohmann::json modelInfo_;
	Vector2F textureSize_;
	std::vector <Model_Part> modelParts_;
	Polygon3DRenderer::PolygonInfo polygonInfo_;
	std::unique_ptr<Polygon3DMaterial> material_;
	std::shared_ptr<Polygon3DRenderer> renderer_;

	void LoadModelInfo(void);
	void LoadSkin(std::string skinName);
	void MakePokygonInfo(void);
};

