#include "DragonActionUpdateBase.h"

DragonActionUpdateBase::DragonActionUpdateBase(std::weak_ptr<Transform> dragonTrans, nlohmann::json json, std::weak_ptr<DragonAction>parent)
{
	dragonTrans_ = dragonTrans;
	json_ = json;
	parent_ = parent;
}

DragonActionUpdateBase::~DragonActionUpdateBase(void)
{
}
