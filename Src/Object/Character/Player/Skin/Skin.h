#pragma once
#include <string>

class Skin
{
public:
	Skin(std::string name,std::string path,bool isPreload);
	~Skin(void);

	//ロードされているか
	const bool isLoaded(void) const;
	void Load(void);

	const int GetHandle(void) const; 

private:
	int handle_ = -1;	// スキンの画像ハンドル
	std::string path_;
	std::string name_;
};

