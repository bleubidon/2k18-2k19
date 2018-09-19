#pragma once

typedef float vec_type;

struct vec
{
	vec();
	vec(vec_type _x, vec_type _y);

	void set(vec_type _x, vec_type _y);
	vec unit();

	vec& operator+=(const vec& v);

	vec_type x, y;
};

vec_type dist2(const vec& a, const vec& b);
vec_type dot(const vec& a, const vec& b);
vec operator+(const vec& a, const vec& b);
vec operator-(const vec& a, const vec& b);
bool operator==(const vec &a, const vec &b);
vec operator*(vec_type a, const vec& p);