#pragma once
#include <random>

inline double rand01()
{
    static std::random_device               rd;
    static std::mt19937                     gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

// 生成 [min, max] 范围的随机整数（包含边界）
inline int randInt(int min, int max)
{
    static std::random_device          rd;
    static std::mt19937                gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}