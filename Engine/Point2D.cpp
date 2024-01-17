#include <iostream>
#include "Point2D.h"


Point2D::Point2D()
{
	x = 0;
	y = 0;
}

Point2D::Point2D(float i_x, float i_y)
{
	x = i_x;
	y = i_y;
}

float Point2D::getX() { return x; }
float Point2D::getY() { return y; }

void Point2D::setX(float i_x) { x = i_x; }
void Point2D::setY(float i_y) { y = i_y; }

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

Point2D Point2D::operator*=(float other)
{
	x *= other;
	y *= other;
	return Point2D(x, y);
}

Point2D Point2D::operator/=(float other)
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

Point2D operator*(Point2D lhs, float rhs)
{
	return Point2D(lhs.getX() * rhs, lhs.getY() * rhs);
}

Point2D operator*(float lhs, Point2D rhs)
{
	return Point2D(lhs * rhs.getX(), lhs * rhs.getY());
}

Point2D operator/(Point2D lhs, float rhs)
{
	return Point2D(lhs.getX() / rhs, lhs.getY() / rhs);
}

const Point2D Point2D::Up = Point2D(0.0, 1.0);
const Point2D Point2D::Down = Point2D(0.0, -1.0);
const Point2D Point2D::Left = Point2D(-1.0, 0.0);
const Point2D Point2D::Right = Point2D(1.0, 0.0);