#pragma once
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <variant>
class Store;

enum class EventType
{
    SelectLevel
};

struct EventData
{
    std::variant<std::string, int> prop1;
};

class Event
{
public:
    EventType type;
    EventData data;
};

namespace callback {
    /**
     * @brief 选择关卡
     * @note 仅仅设置当前关卡名，并进入加载状态
     * @param store
     * @param level_name [in] 关卡名称
     */
    void SelectLevel(Store& store,const std::string& level_name);
}