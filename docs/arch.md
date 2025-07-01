# 项目结构

项目被我分为 Model, View, Function, Manager 四个层。

## Model 层

Model 层需要维护数据实体，包括 tower, enemy, soldier, bullet, modifier, fx

对于每一个实体，需要实现：

*  `update()`：在每一个游戏刻被调用，实现数据实体的状态转移方程。
*  `insert()`：在数据实体被插入游戏时需要执行的逻辑。
*  `remove()`：在数据实体被移除游戏时需要执行的逻辑。

注意事项：

动画未结束时，除非在行进或者空闲，不应改变数据实体的状态。

## View 层

View 层需要根据数据实体的状态来确定它播放什么动画，并提供确定的动画播放逻辑。它需要告知实体，当前动画是否已经播放完毕。

View 层还需要提供声音播放逻辑，以便 Model 层与 Manager 层调用。

所有美术资源向 Manager 层请求。

View 层需要实现各个可以接受输入事件的可视模块，如按钮，并让它们捕获用户的输入事件，传递给游戏的全局变量，通过 Manager 层进一步处理。

## Function 层

实现具体的行为逻辑，如：

* 伤害的结算函数
* 生命值的恢复函数
* 位置的更新函数
* 护甲值的更新函数
* buff 的更新函数
* 各种点击事件的回调函数

## Manager 层

Manager 层需要确认总的游戏状态，如：

* 暂停
* 选择关卡
* 战斗中
* 结算

Manager 层需要编写主线程函数和副线程函数。

Manager 层需要管理所有的美术资源。

Manager 层需要维护游戏的全局变量。

## ADD ON

UIManager 提供接口：

```cpp
QueueEnemyUI(Enemy*)
QueueSoldierUI(Soldier*)
QueueTowerUI(Tower*)
QueueBulletUI(Bullet*)
DeQueueEnemyUI(Enemy*)
// and so on
```

## 渲染方案

`Animation` 的成员加上 `Position` 足够完成全部的渲染工作。

```cpp
    State       state;
    State       last_state;
    double      rotation = 0.0;    // 旋转角度
    bool        pending  = true;   // 动画正在进行
    bool        flip     = false;
    std::size_t frame_id = 0;   // 动画进行到第几帧
    std::string prefix;         // 动画的前缀名
```

对于 `Layer` ，拥有

```cpp
struct Layer{
    Animation animation;
    Position offset;
};
```

所以只要将主体的 `postion` 加上它的 `offset` 就可以完成渲染。

因此，最普遍的接口是一个接受 `const Position&` , `Animation&` 的渲染函数。
