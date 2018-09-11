#include "PathFinding.h"
#include <math.h>

inline int sq(int x)
{ return x * x; }

inline float sq(float x)
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


bool Path::find(int x, int y)
{
	length = 0;
	current = 1;

	int figure = 1;
	if (figure == 0) {
		waypoints[length++].set(x, y);
		waypoints[length++].set(x + 10, y);
	}
	if (figure == 1) {
		waypoints[length++].set(x, y);
		waypoints[length++].set(x, y + 100);
	}
	else if (figure == 2) {
		for (float angle = 0; angle < 3.1415; angle += 0.4f) {
			waypoints[length++].set(x + 5 * sin(angle), y - 5 * cos(angle) + 5);
		}
	}
	return true;
}


float tj(vec a, vec b)
{
	const static float alpha = 1;
	return pow( sq(b.x-a.x) + sq(b.y-a.y), alpha / 2 );
}

vec dCatmullRomSpline(vec p[4])
{
	float t[] = {0, 0, 0, 0};
	for (int i = 0; i < 3; i++)
		t[i+1] = tj(p[i], p[i+1]) + t[i];
		
	float s = t[1];
	vec A[3], dA[3], B[2], dB[2];
	for (int i = 0; i < 3; i++)
	{
		float coef = 1 / (t[i+1] - t[i]);
		float coef1 = (t[i+1] - s) * coef;
		float coef2 = (s - t[i]) * coef;
		A[i].set(coef1*p[i].x + coef2*p[i+1].x, coef1*p[i].y + coef2*p[i+1].y);
		// A[i] = coef1 * p[i] + coef2 * p[i + 1];
		dA[i] = coef * (p[i + 1] -  p[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		float coef = 1 / (t[i+2] - t[i]);
		float coef1 = (t[i+2] - s) * coef;
		float coef2 = (s - t[i]) * coef;
		B[i].set(coef1*A[i].x + coef2*A[i+1].x, coef1*A[i].y + coef2*A[i+1].y);
		// B[i] = coef1 * A[i] + coef2 * A[i + 1];
		dB[i] = coef * (A[i+1] - A[i]) + coef1 * dA[i] + coef2 * dA[i+1];
	}

	float coef = 1 / (t[2] - t[1]);
	float coef1 = (t[2] - s) * coef;
	float coef2 = (s - t[1]) * coef;
	return (coef * (B[1] - B[0])) + (coef1 * dB[0]) + (coef2 * dB[1]);
}

vec Path::get_direction(const vec& pos, const vec& dir)
{
	if (dist2( pos, waypoints[current]) < 25)
		return vec(0, 0);
	return (waypoints[current] - pos).unit();
}