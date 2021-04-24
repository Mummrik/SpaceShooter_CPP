#pragma once

#include <cmath>

struct Vec2d
{
	Vec2d(float x = 0, float y = 0) : x(x), y(y) {}
	float x, y;

	Vec2d& operator += (const Vec2d& rhs)
	{
		if (this != &rhs)
		{
			x += rhs.x;
			y += rhs.y;
		}

		return *this;
	}

	Vec2d& operator -= (const Vec2d& rhs)
	{
		if (this != &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		}

		return *this;
	}

	float Magnitude()
	{
		return sqrt((x * x) + (y * y));
	}
};

Vec2d operator - (const Vec2d& lhs, const Vec2d& rhs);
Vec2d operator + (const Vec2d& lhs, const Vec2d& rhs);
Vec2d operator * (const Vec2d& lhs, const float& value);

