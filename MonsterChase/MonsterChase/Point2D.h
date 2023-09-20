#pragma once

class Point2D
{
	int x;
	int y;

public:
	Point2D();
	Point2D(int i_x, int i_y);

	int getX();
	int getY();
	void setX(int i_x);
	void setY(int i_y);

	void print();
	
	Point2D operator+=(Point2D other);
	Point2D operator-=(Point2D other);
	Point2D operator*=(Point2D other);
	Point2D operator/=(Point2D other);
	bool operator!=(Point2D other);
	bool operator==(Point2D other);

	~Point2D() {}

	static const Point2D Up;
	static const Point2D Down;
	static const Point2D Left;
	static const Point2D Right;
};

Point2D operator+(Point2D lhs, Point2D rhs);
Point2D operator-(Point2D lhs, Point2D rhs);
Point2D operator*(Point2D lhs, Point2D rhs);
Point2D operator/(Point2D lhs, Point2D rhs);