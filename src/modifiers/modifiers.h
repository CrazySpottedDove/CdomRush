#pragma once
#include "../store/store.h"
#include "../utils/unit.h"
#include <cstddef>
#include <memory>
class Modifier
{
public:
    std::shared_ptr<Unit> target;
    std::size_t           sprite_id;
    virtual void          Insert(Store& store);
    virtual void          Update(Store& store);
    virtual void          Remove(Store& store);
};
