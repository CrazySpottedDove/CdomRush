#include "ViewModel/GameViewModel/templates/entity.h"
#include "Common/macros.h"
#include "ViewModel/GameViewModel/store/store.h"

bool Entity::Insert(Store& store)
{
    return true;
}
void Entity::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(&animations, position, COMMON_LAYER);
}
