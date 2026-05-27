#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../../../../Lib/nlohmann/json.hpp"

class Skin;

class SkinManager
{
public:
	static void CreateInstance(void);
	static SkinManager& GetInstance(void);
	void Init(void);
	void Release(void);
	void Destroy(void);

	const int GetHandleId(const std::string key);
	const int GetSkinNum(void);

private:

	static SkinManager* instance_;
	
	std::map<std::string,std::shared_ptr<Skin>> skinResources_;
	nlohmann::json skinJson_;

	SkinManager() = default;
	~SkinManager() = default;
};

