#include "DragonActionUpdateFlyMove.h"

DragonActionUpdateFlyMove::DragonActionUpdateFlyMove(std::weak_ptr<Transform> dragonTrans, nlohmann::json json):DragonActionUpdateBase(dragonTrans,json)
{

}

DragonActionUpdateFlyMove::~DragonActionUpdateFlyMove(void)
{
}

void DragonActionUpdateFlyMove::Update(void)
{
}
