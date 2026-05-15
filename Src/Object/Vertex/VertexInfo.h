#pragma once
#include <string>
#include "../../Lib/nlohmann/json.hpp"
#include "../../Renderer/Polygon3DRenderer.h"

class VertexInfo
{
public:
	struct UVOffset
	{
		Vector2F leftUp;
		Vector2F rightDown;
	};

	struct Cube_Param
	{
		std::string filePath;
		VECTOR center;
		VECTOR cubeSize;
		Vector2F TextureSize;
		Vector2F startUV;
	};

	static Polygon3DRenderer::PolygonInfo LoadFromFile(const Cube_Param& param);
private:
	static UVOffset GetUVOffset(const nlohmann::json offset,VECTOR size);
	static Vector2F GetUVOffset(const nlohmann::json offset,const std::string key,VECTOR size);
	static float CalcUVOffset(const std::string offset, const VECTOR size);
};

