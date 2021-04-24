#include "Vec2d.h"

Vec2d operator + (const Vec2d& lhs, const Vec2d& rhs)
{
	Vec2d ret = lhs;
	ret += rhs;
	return ret;
}

Vec2d operator*(const Vec2d& lhs, const float& value)
{
	Vec2d ret = lhs;
	ret.x *= value;
	ret.y *= value;
	return ret;
}


Vec2d operator - (const Vec2d& lhs, const Vec2d& rhs)
{
	Vec2d ret = lhs;
	ret -= rhs;
	return ret;
}