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
	bool operator!=(Point2D other);
	bool operator==(Point2D other);

	~Point2D() {}
};

Point2D operator+(Point2D lhs, Point2D rhs);
Point2D operator-(Point2D lhs, Point2D rhs);