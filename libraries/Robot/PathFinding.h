#pragma once

#define MAX_WAYPOINTS 20

struct Point
{
	Point();
	Point(int _x, int _y);

	void set(int _x, int _y);
	Point unit();

	Point operator+( const Point& p );
	Point operator-( const Point& p );
	int x, y;
};

int dist2(const Point& a, const Point& b);
int dot(const Point& a, const Point& b);
bool operator==(const Point &a, const Point &b);
Point operator*(int a, const Point& p);


struct Path
{
	bool find(int x, int y);
	Point get_direction(const Point& pos, const Point& dir);

	private:
		Point waypoints[MAX_WAYPOINTS];
		int current, length;
};