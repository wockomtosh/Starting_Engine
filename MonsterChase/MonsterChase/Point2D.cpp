#include <iostream>

class Point2D 
{
	int x;
	int y;

public:	
	Point2D(int i_x = 0, int i_y = 0)
	{
		x = i_x;
		y = i_y;
	}

	int x() { return x; }
	int y() { return y; }

	void x(int i_x) { x = i_x; }
	void y(int i_y) { y = i_y; }

	Point2D operator+=(Point2D other)
	{
		x += other.x;
		y += other.y;
	}

	void print()
	{
		std::cout << "[" << x << ", " << y << "]";
	}
};

Point2D operator+(Point2D lhs, Point2D rhs)
{
	return Point2D(lhs.x() + rhs.x(), lhs.x() + rhs.x());
}

Point2D operator-(Point2D lhs, Point2D rhs)
{
	return Point2D(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}