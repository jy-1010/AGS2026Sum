#pragma once
#include <string>
#include <vector>
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
		std::string key;
		VECTOR center;
		VECTOR cubeSize;
		Vector2F TextureSize;
		Vector2F startUV;
	};

	struct InputVerticesParam
	{
		VECTOR pos;
		Vector2F localUV;
		VECTOR normal;
	};
	static Polygon3DRenderer::PolygonInfo LoadFromFile(const Cube_Param& param);
	static float GetPixelSize(std::string key);
private:
	static UVOffset GetUVOffset(const nlohmann::json offset,VECTOR size);
	static Vector2F GetUVOffset(const nlohmann::json offset,const std::string key,VECTOR size);
	static float CalcUVOffset(const std::string offset, const VECTOR size);
	static std::vector<InputVerticesParam> GetInputVertices(const nlohmann::json vertices);
	static VERTEX3DSHADER CreateVertex(const InputVerticesParam& inputParam, const Cube_Param& cubeParam,const UVOffset& uvoffset,float size);
};

