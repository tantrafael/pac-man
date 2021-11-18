#include "Vector2Int.h"

#include <math.h>

Vector2Int::Vector2Int()
: x(0)
, y(0)
{};

Vector2Int::Vector2Int(const int x, const int y)
: x(x)
, y(y)
{};

Vector2Int Vector2Int::operator + (const Vector2Int& v) const
{
    return Vector2Int(x + v.x, y + v.y);
}

Vector2Int Vector2Int::operator - (const Vector2Int& v) const
{
    return Vector2Int(x - v.x, y - v.y);
}

Vector2Int Vector2Int::operator * (const int n) const
{
    return Vector2Int(x * n, y * n);
}

Vector2Int Vector2Int::operator - () const
{
    return Vector2Int(-x, -y);
}

bool Vector2Int::operator == (const Vector2Int& v) const
{
    return ((x == v.x) && (y == v.y));
}

bool Vector2Int::operator != (const Vector2Int& v) const
{
    return !(*this == v);
}

void Vector2Int::operator += (const Vector2Int& v)
{
    *this = *this + v;
}

void Vector2Int::operator -= (const Vector2Int& v)
{
    *this = *this - v;
}

Vector2Int::operator Vector2 () const
{
    return Vector2(static_cast<float>(x), static_cast<float>(y));
}

float Vector2Int::SqrMagnitude()
{
    return static_cast<float>(x * x + y * y);
}

float Vector2Int::Magnitude()
{
    return static_cast<float>(sqrt(this->SqrMagnitude()));
}

Vector2Int Vector2Int::Zero = Vector2Int(0, 0);
Vector2Int Vector2Int::One = Vector2Int(1, 1);
Vector2Int Vector2Int::Left = Vector2Int(-1, 0);
Vector2Int Vector2Int::Right = Vector2Int(1, 0);
Vector2Int Vector2Int::Down = Vector2Int(0, -1);
Vector2Int Vector2Int::Up = Vector2Int(0, 1);
