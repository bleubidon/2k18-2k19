#pragma once

#include "vec.h"

#define MAX_WAYPOINTS 20

struct Path
{
	int find(const vec& start);
	int get_distance(const vec& pos);
	vec get_direction(const vec& pos, const vec& dir);

	private:
		vec waypoints[MAX_WAYPOINTS];
		int current, length;
};