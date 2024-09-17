#ifndef POINT_H_
#define POINT_H_
#include <vector>
#include <iostream>

struct Point {
	int x;
	int y;
	int z;
	Point operator+=(const Point& a);
};

struct Path{
	std::vector<Point> cells;
	bool circle;
};

bool operator==(const Point& a, const Point& b);
Point operator-(const Point& a, const Point& b);
Point operator+(const Point& a, const Point& b);
std::ostream& operator <<(std::ostream& str, const Point& point);

#endif
