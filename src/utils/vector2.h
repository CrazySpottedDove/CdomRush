#pragma once
#include <cmath>
struct Vector2
{
    float x;
    float y;

    Vector2() noexcept
        : x(0)
        , y(0)
    {}
    Vector2(double x, double y) noexcept
        : x(x)
        , y(y)
    {}

    Vector2(const Vector2& other) noexcept
        : x(other.x)
        , y(other.y)
    {}

    Vector2(double value) noexcept
        : x(value)
        , y(value)
    {}

    Vector2 operator+(const Vector2& other) const noexcept { return Vector2(x + other.x, y + other.y); }

    Vector2 operator-(const Vector2& other) const noexcept{ return Vector2(x - other.x, y - other.y); }

    Vector2 operator*(double scalar) const noexcept{ return Vector2(x * scalar, y * scalar); }

    double Length() const { return std::sqrt(x * x + y * y); }
};
