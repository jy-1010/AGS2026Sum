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

	//初期化
	void Init(void);
	//解放
	void Release(void);
	//インスタンス破壊
	void Destroy(void);

	//画像のハンドルIDを取得する
	const int GetHandleId(const std::string key);
	//今登録されているスキンの数を取得する
	const int GetSkinNum(void);

	//スキンをマネージャーに保存セーブする
	void SaveSkin(const std::string name);

	//スキンのパスを取得
	std::string GetRootPath(void) { return rootPath_; }
private:

	static SkinManager* instance_;
	
	std::map<std::string,std::shared_ptr<Skin>> skinResources_;
	nlohmann::json skinJson_;

	std::string rootPath_;

	void AddSkin(const std::string name, const std::string path, bool isPreload);
	//新規スキンをjsonに保存
	void AddSkinJson(const std::string name,const std::string path);

	SkinManager() = default;
	~SkinManager() = default;
};

