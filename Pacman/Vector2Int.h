#pragma once

#include "Vector2.h"

struct Vector2Int
{
    int x;
    int y;

    Vector2Int();
    Vector2Int(const int x, const int y);

    Vector2Int operator + (const Vector2Int& v) const;
    Vector2Int operator - (const Vector2Int& v) const;
    Vector2Int operator * (const int n) const;
    Vector2Int operator - () const;
    bool operator == (const Vector2Int& v) const;
    bool operator != (const Vector2Int& v) const;
	void operator += (const Vector2Int& v);
    void operator -= (const Vector2Int& v);
	operator Vector2 () const;
    float SqrMagnitude();
    float Magnitude();

    static Vector2Int Zero, One, Left, Right, Down, Up;
};
