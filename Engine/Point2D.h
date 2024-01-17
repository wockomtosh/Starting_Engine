#pragma once

class Point2D
{
	

public:
	float x;
	float y;

	Point2D();
	Point2D(float i_x, float i_y);

	float getX();
	float getY();
	void setX(float i_x);
	void setY(float i_y);

	void print();
	
	Point2D operator+=(Point2D other);
	Point2D operator-=(Point2D other);
	Point2D operator*=(Point2D other);
	Point2D operator/=(Point2D other);
	Point2D operator*=(float other);
	Point2D operator/=(float other);
	Point2D operator-();
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
Point2D operator*(Point2D lhs, float rhs);
Point2D operator*(float lhs, Point2D rhs);
Point2D operator/(Point2D lhs, float rhs);