#pragma once
#include <random>

inline double rand01()
{
    static std::random_device               rd;
    static std::mt19937                     gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}
