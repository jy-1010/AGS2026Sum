#pragma once
#include <vector>

class DragonBreath
{
public:

	enum class STATE
	{
		FLIGHT,	//空中状態
		SPRASH,	//着地後の拡散
		POISON_ARER,	//毒エリア
		FINISH,	//終了
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	DragonBreath(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~DragonBreath(void);

private:

};

