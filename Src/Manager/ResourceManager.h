#pragma once
#include<string>
#include<map>
#include<memory>

#include "Resource/Resource.h"

class ResourceManager
{	
public:
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);
private:
	// 静的インスタンス
	static ResourceManager* instance_;
	std::map<std::string, std::shared_ptr<Resource>> resourcesMap_;
	std::map<std::string, std::vector<std::shared_ptr<Resource>>> loadedMap_;

	Resource::TYPE GetResourceTypeFromString(const std::string&	str);
	std::shared_ptr<Resource> CreateResource( const nlohmann::json& json, Resource::TYPE type);
};

