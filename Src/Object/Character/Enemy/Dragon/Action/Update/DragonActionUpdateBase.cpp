#include "DragonActionUpdateBase.h"

DragonActionUpdateBase::DragonActionUpdateBase(std::weak_ptr<Transform> dragonTrans, nlohmann::json json)
{
	dragonTrans_ = dragonTrans;
	json_ = json;
}

DragonActionUpdateBase::~DragonActionUpdateBase(void)
{
}
