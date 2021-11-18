#include "Vector2.h"

#include <math.h>

Vector2::Vector2()
: x(0)
, y(0)
{};

Vector2::Vector2(const float x, const float y)
: x(x)
, y(y)
{};

Vector2::Vector2(const int x, const int y)
: x(static_cast<float>(x))
, y(static_cast<float>(y))
{};

Vector2 Vector2::operator + (const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator - (const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator * (const float t) const
{
    return Vector2(x * t, y * t);
}

Vector2 Vector2::operator / (const float t) const
{
    return *this * (1.0f / t);
}

Vector2 Vector2::operator - () const
{
    return Vector2(-x, -y);
}

bool Vector2::operator == (const Vector2& v) const
{
    return (x == v.x) && (y == v.y);
}

bool Vector2::operator != (const Vector2& v) const
{
    return !(*this == v);
}

void Vector2::operator += (const Vector2& v)
{
    *this = *this + v;
}

void Vector2::operator -= (const Vector2& v)
{
    *this = *this - v;
}

float Vector2::SqrMagnitude()
{
    return x * x + y * y;
}

float Vector2::Magnitude()
{
    return static_cast<float>(sqrt(this->SqrMagnitude()));
}

Vector2 Vector2::Normalized()
{
	Vector2 normalized = *this;

	if (*this != Zero)
	{
		normalized = *this / this->Magnitude();
	}

	return normalized;
}

void Vector2::Normalize()
{
	if (*this != Zero)
	{
		*this = *this / this->Magnitude();
	}
}

Vector2 Vector2::Zero = Vector2(0, 0);
Vector2 Vector2::One = Vector2(1, 1);
Vector2 Vector2::Left = Vector2(-1, 0);
Vector2 Vector2::Right = Vector2(1, 0);
Vector2 Vector2::Down = Vector2(0, -1);
Vector2 Vector2::Up = Vector2(0, 1);
