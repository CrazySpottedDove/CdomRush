#pragma once
struct Armor
{
    Armor(double physical = 0.0, double magical = 0.0)
        : physical(physical), magical(magical) {};
    double physical   = 0;
    double magical    = 0;
    double poison     = 0;
    double fire       = 0;
    double electrical = 0;
    double blood      = 0;
};
