#pragma once

#define MAX_WAYPOINTS 20
typedef float vec_type;

struct vec
{
	vec();
	vec(vec_type _x, vec_type _y);

	void set(vec_type _x, vec_type _y);
	vec unit();

	vec_type x, y;
};

vec_type dist2(const vec& a, const vec& b);
vec_type dot(const vec& a, const vec& b);
vec operator+(const vec& a, const vec& b);
vec operator-(const vec& a, const vec& b);
bool operator==(const vec &a, const vec &b);
vec operator*(vec_type a, const vec& p);

struct Path
{
	int find(const vec& start);
	int get_distance(const vec& pos);
	vec get_direction(const vec& pos, const vec& dir);

	private:
		vec waypoints[MAX_WAYPOINTS];
		int current, length;
};