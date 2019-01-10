#include "vec.h"
#include <math.h>

inline static int sq(int x)
{ return x * x; }

inline static float sq(float x)
{ return x * x; }


vec::vec()
{}

vec::vec(float radians):
	x(cos(radians)), y(sin(radians))
{}

vec::vec(vec_type _x, vec_type _y):
	x(_x), y(_y)
{}

void vec::set(vec_type _x, vec_type _y)
{
	x = _x;
	y = _y;
}

vec_type vec::angle()
{
	return atan2(y, x) * 180.f / M_PI;
}

vec vec::unit()
{
	float length = sqrt( sq(x) + sq(y) );
	return vec(x / length, y / length);
}

vec& vec::operator+=(const vec& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

vec_type vec::dist(const vec& a, const vec& b)
{
	return sqrt(dist2(a, b));
}

vec_type vec::dist2(const vec& a, const vec& b)
{
	return sq(a.x - b.x) + sq(a.y - b.y);
}

vec_type vec::dot(const vec& a, const vec& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

vec operator+(const vec &a, const vec &b)
{
	return vec(a.x + b.x, a.y + b.y);
}

vec operator-(const vec &a, const vec &b)
{
	return vec(a.x - b.x, a.y - b.y);
}

bool operator==(const vec &a, const vec &b)
{
	return a.x == b.x && a.y == b.y;
}

vec operator*(vec_type a, const vec &p)
{
	return vec(a * p.x, a * p.y);
}
