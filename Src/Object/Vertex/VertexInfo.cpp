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
		//情報を読み込む
		auto& vertex = Vertices[Order[i]];
		UVOffset uvOffset = GetUVOffset(vertex["UVOffset"],param.cubeSize);
		auto inputVertices = GetInputVertices(vertex["Vertices"]);
		//頂点情報を作成
		for (int j = 0;j < inputVertices.size();j++)
		{
			polygonInfo.vertex.push_back(CreateVertex(inputVertices[j], param, uvOffset));
		}
		polygonInfo.Indices.push_back(i * 4);
		polygonInfo.Indices.push_back(i * 4 + 1);
		polygonInfo.Indices.push_back(i * 4 + 2);
		polygonInfo.Indices.push_back(i * 4);
		polygonInfo.Indices.push_back(i * 4 + 2);
		polygonInfo.Indices.push_back(i * 4 + 3);
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

std::vector<VertexInfo::InputVerticesParam> VertexInfo::GetInputVertices(const nlohmann::json vertices)
{
	std::vector<InputVerticesParam> inputVertices;
	for (auto& param : vertices)
	{
		InputVerticesParam vertex;
		auto& pos = param["Pos"];
		vertex.pos = VECTOR(pos["x"], pos["y"], pos["z"]);
		auto& localUV = param["LocalUV"];
		vertex.localUV = Vector2F(localUV["u"], localUV["v"]);
		auto& normal = param["Normal"];
		vertex.normal = VECTOR(normal["x"], normal["y"], normal["z"]);
		inputVertices.push_back(vertex);
	}
	return inputVertices;
}

VERTEX3DSHADER VertexInfo::CreateVertex(const InputVerticesParam& inputParam, const Cube_Param& cubeParam, const UVOffset& uvoffset)
{
	const VECTOR center = cubeParam.center;
	VERTEX3DSHADER ret;
	VECTOR posOffset = VECTOR(center.x * inputParam.pos.x, center.y * inputParam.pos.y, center.z * inputParam.pos.z);
	ret.pos =VAdd(cubeParam.center, posOffset);
	ret.spos = FLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	ret.norm = inputParam.normal;
	ret.tan = VECTOR(0.0f, 0.0f, 0.0f);
	ret.binorm = VECTOR(0.0f, 0.0f, 0.0f);
	ret.dif = COLOR_U8(255, 255, 255, 255);
	ret.spc = COLOR_U8(0, 0, 0, 255);
	float u = cubeParam.startUV.u + ((uvoffset.rightDown.u - uvoffset.leftUp.u) * inputParam.localUV.u + uvoffset.leftUp.u);
	float v = cubeParam.startUV.v + ((uvoffset.rightDown.v - uvoffset.leftUp.v) * inputParam.localUV.v + uvoffset.leftUp.v);
	ret.u = u / cubeParam.TextureSize.u;
	ret.v = v / cubeParam.TextureSize.v;
	ret.su = ret.u;
	ret.sv = ret.v;
	return ret;
}
