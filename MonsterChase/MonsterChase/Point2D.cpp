#include <iostream>
#include "Point2D.h";


Point2D::Point2D()
{
	x = 0;
	y = 0;
}

Point2D::Point2D(int i_x, int i_y)
{
	x = i_x;
	y = i_y;
}

int Point2D::getX() { return x; }
int Point2D::getY() { return y; }

void Point2D::setX(int i_x) { x = i_x; }
void Point2D::setY(int i_y) { y = i_y; }

void Point2D::print()
{
	std::cout << "[" << x << ", " << y << "]";
}

Point2D Point2D::operator+=(Point2D other)
{
	x += other.x;
	y += other.y;
	return Point2D(x, y);
}

Point2D Point2D::operator-=(Point2D other)
{
	x -= other.x;
	y -= other.y;
	return Point2D(x, y);
}

Point2D Point2D::operator*=(Point2D other)
{
	x *= other.x;
	y *= other.y;
	return Point2D(x, y);
}

Point2D Point2D::operator/=(Point2D other)
{
	x /= other.x;
	y /= other.y;
	return Point2D(x, y);
}

Point2D Point2D::operator*=(int other)
{
	x *= other;
	y *= other;
	return Point2D(x, y);
}

Point2D Point2D::operator/=(int other)
{
	x /= other;
	y /= other;
	return Point2D(x, y);
}

Point2D Point2D::operator-()
{
	return Point2D(-x, -y);
}


bool Point2D::operator!=(Point2D other)
{
	return (y != other.getX() && y != other.getY());
}

bool Point2D::operator==(Point2D other)
{
	return (x == other.getX() && y == other.getY());
}



Point2D operator+(Point2D lhs, Point2D rhs)
{
	return Point2D(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY());
}

Point2D operator-(Point2D lhs, Point2D rhs)
{
	return Point2D(lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY());
}

Point2D operator*(Point2D lhs, Point2D rhs)
{
	return Point2D(lhs.getX() * rhs.getX(), lhs.getY() * rhs.getY());
}

Point2D operator/(Point2D lhs, Point2D rhs)
{
	return Point2D(lhs.getX() / rhs.getX(), lhs.getY() / rhs.getY());
}

Point2D operator*(Point2D lhs, int rhs)
{
	return Point2D(lhs.getX() * rhs, lhs.getY() * rhs);
}

Point2D operator*(int lhs, Point2D rhs)
{
	return Point2D(lhs * rhs.getX(), lhs * rhs.getY());
}

Point2D operator/(Point2D lhs, int rhs)
{
	return Point2D(lhs.getX() / rhs, lhs.getY() / rhs);
}

const Point2D Point2D::Up = Point2D(0, 1);
const Point2D Point2D::Down = Point2D(0, -1);
const Point2D Point2D::Left = Point2D(-1, 0);
const Point2D Point2D::Right = Point2D(1, 0);