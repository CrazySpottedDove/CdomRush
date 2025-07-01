#include "Function/events/events.h"
#include "Manager/store/store.h"

// 任何方法都不应当直接修改 Store 的状态。 Store 的状态只在它自身的成员函数中被修改。
namespace callback {
    void SelectLevel(Store& store, const std::string& level_name) {
        store.level_manager.SetCurrentLevelName(level_name);
    }
}