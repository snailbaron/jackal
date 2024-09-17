#include <ostream>
#include "Point.h"

bool operator==(const Point& a, const Point& b)
{
	return ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z));
}

Point Point::operator+=(const Point& a)
{
	this->x+=a.x;
	this->y+=a.y;
	z = 1;
	return (*this);
}

Point operator-(const Point& a, const Point& b)
{
	Point ans;
	ans.x = a.x - b.x;
	ans.y = a.y - b.y;
	ans.z = 1;
	return ans;
}

Point operator+(const Point& a, const Point& b)
{
	Point ans;
	ans.x = a.x + b.x;
	ans.y = a.y + b.y;
	ans.z = 1;
	return ans;
}

std::ostream& operator <<(std::ostream& out,const Point& point)
{
	out<<point.x<<" "<<point.y<<" "<<point.z;
	return out;
}