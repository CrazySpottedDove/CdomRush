struct Health
{
    double hp_max;
    double hp;
    // 尸体保持的时间
    Health(double hp_max = 100.0, double hp = 100.0)
        : hp_max(hp_max), hp(hp) {};
    double dead_lifetime = 3.0;
    double death_time = 0.0;
    // 是否免疫伤害
    bool immune = false;
    double regen_cooldown = 1.0; // 恢复冷却时间
    double regen_factor = 0.2; // 每秒恢复的生命值比例
    double last_regen_time = 0.0;
};