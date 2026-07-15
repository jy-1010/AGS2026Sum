#include "DragonActionUpdateFlyMove.h"

DragonActionUpdateFlyMove::DragonActionUpdateFlyMove(std::weak_ptr<Transform> dragonTrans, nlohmann::json json, std::weak_ptr<DragonAction>parent)
	:DragonActionUpdateBase(dragonTrans,json,parent)
{

}

DragonActionUpdateFlyMove::~DragonActionUpdateFlyMove(void)
{
}

void DragonActionUpdateFlyMove::Update(void)
{

}

void DragonActionUpdateFlyMove::Load(void)
{
}
