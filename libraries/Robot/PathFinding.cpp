#include "PathFinding.h"
#include "math.h"

Point::Point()
{}

Point::Point(int _x, int _y):
	x(_x), y(_y)
{}

void Point::set(int _x, int _y)
{
	x = _x;
	y = _y;
}

Point Point::unit()
{
	float length = sqrt(x * x + y * y);
	return Point(x / length, y / length);
}

Point Point::operator+(const Point &p)
{
	return Point(p.x + x, p.y + y);
}

Point Point::operator-(const Point &p)
{
	return Point(p.x - x, p.y - y);
}

int dot(const Point& a, const Point& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

bool operator==(const Point &a, const Point &b)
{
	return a.x == b.x && a.y == b.y;
}

Point operator*(int a, const Point &p)
{
	return Point(a * p.x, a * p.y);
}


bool Path::find(int x, int y)
{
	length = 0;
	current = 1;

	int figure = 0;
	if (figure == 0) {
		waypoints[length++].set(x, y);
		waypoints[length++].set(x + 10, y);
	}
	return true;
}

Point Path::get_direction(const Point& pos, const Point& dir)
{
	return (waypoints[current] - pos).unit();
}