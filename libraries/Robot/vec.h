#pragma once

typedef float vec_type;

struct vec
{
	vec();
	vec(float radians);
	vec(vec_type _x, vec_type _y);

	void set(vec_type _x, vec_type _y);
	vec unit();

	vec& operator+=(const vec& v);

	static vec_type dist(const vec& a, const vec& b);
	static vec_type dist2(const vec& a, const vec& b);
	static vec_type dot(const vec& a, const vec& b);

	vec_type x, y;
};

vec operator+(const vec& a, const vec& b);
vec operator-(const vec& a, const vec& b);
bool operator==(const vec &a, const vec &b);
vec operator*(vec_type a, const vec& p);
