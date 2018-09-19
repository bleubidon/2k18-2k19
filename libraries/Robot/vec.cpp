#include "vec.h"
#include <math.h>

inline static int sq(int x)
{ return x * x; }

inline static float sq(float x)
{ return x * x; }


vec::vec()
{}

vec::vec(vec_type _x, vec_type _y):
	x(_x), y(_y)
{}

void vec::set(vec_type _x, vec_type _y)
{
	x = _x;
	y = _y;
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

vec_type dist2(const vec& a, const vec& b)
{
	return sq(a.x - b.x) + sq(a.y - b.y);
}

vec_type dot(const vec& a, const vec& b)
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