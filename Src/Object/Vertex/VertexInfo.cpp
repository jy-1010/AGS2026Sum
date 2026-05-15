#include <fstream>
#include <vector>
#include "VertexInfo.h"

Polygon3DRenderer::PolygonInfo VertexInfo::LoadFromFile(const Cube_Param& param)
{
	Polygon3DRenderer::PolygonInfo polygonInfo;
	polygonInfo.clear();
	// ファイルからjsonを読み込む
	std::ifstream file(param.filePath);
	nlohmann::json jsonfile;
	file >> jsonfile;
	// どの面から作るかを読み込み
	std::vector<std::string> Order;
	for (auto& param : jsonfile["Order"])
	{
		Order.push_back(param);
	}
	//頂点情報を面ごとに読み込む
	auto& Vertices = jsonfile["Vertices"];
	for (int i = 0; i < Order.size(); i++)
	{
		auto& vertex = Vertices[Order[i]];
		UVOffset uvOffset = GetUVOffset(vertex["UVOffset"],param.cubeSize);
		VERTEX3DSHADER vertexInfo;
		
	}
	return  polygonInfo;
}

VertexInfo::UVOffset VertexInfo::GetUVOffset(const nlohmann::json offset, VECTOR size)
{
	UVOffset uvOffset;
	uvOffset.leftUp = GetUVOffset(offset, "LeftUp", size);
	uvOffset.rightDown = GetUVOffset(offset, "RightDown", size);
	return uvOffset;
}

Vector2F VertexInfo::GetUVOffset(const nlohmann::json offset, const std::string key, VECTOR size)
{
	auto& param = offset[key];
	Vector2F uvOffset;
	uvOffset.u = CalcUVOffset(param["u"], size);
	uvOffset.v = CalcUVOffset(param["v"], size);
	return uvOffset;
}

float VertexInfo::CalcUVOffset(const std::string offset, const VECTOR size)
{
	float ret = 0.0f;
	for (auto& param : offset)
	{
		if (param == 'x')
		{
			ret += size.x;
		}
		else if (param == 'y')
		{
			ret += size.y;
		}
		else if (param == 'z')
		{
			ret += size.z;
		}
	}
	return ret;
}
