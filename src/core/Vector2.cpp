#include <cmath>

#include "core/Vector2.hpp"

const Vector2 Vector2::UP(0, -1);
const Vector2 Vector2::DOWN(0, 1);
const Vector2 Vector2::LEFT(-1, 0);
const Vector2 Vector2::RIGHT(1, 0);
const Vector2 Vector2::ZERO(0, 0);


Vector2::Vector2(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;

    return *this;
}

Vector2 Vector2::operator + (const Vector2& v) const
{
     return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator - (const Vector2& v) const
{
     return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator * (const Vector2& v) const
{
     return Vector2(x * v.x, y *  v.y);
}

Vector2 Vector2::operator / (const Vector2& v) const
{
     return Vector2(x / v.x, y / v.y);
}

Vector2 Vector2::operator % (const Vector2& v) const
{
     return Vector2(x % v.x, y % v.y);
}

double Vector2::Distance(Vector2& v1, Vector2& v2)
{
    int dx = v2.x - v1.x;
    int dy = v2.y - v1.y;
    double dist = std::sqrt(dx * dx + dy * dy);
    return std::fabs(dist);
}

