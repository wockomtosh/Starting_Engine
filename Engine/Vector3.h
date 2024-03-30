#pragma once

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float i_x, float i_y, float i_z);
	//copy constructor
	Vector3(const Vector3& other);

	float getMagnitude() const;
	Vector3 normalize() const;
	//Vector3 getVectorWithMagnitude(float magnitude) const;
	bool isWithinRangeOf(Vector3 target, float range) const;

	void print();

	float dot(Vector3 other);

	Vector3 operator+=(Vector3 other);
	Vector3 operator-=(Vector3 other);

	Vector3 operator*=(float other);
	Vector3 operator/=(float other);

	Vector3 operator-();

	bool operator!=(Vector3 other);
	bool operator==(Vector3 other);

	static const Vector3 NULL_VECTOR;
};

Vector3 operator+(Vector3 lhs, Vector3 rhs);
Vector3 operator+(Vector3 lhs, float rhs);
Vector3 operator-(Vector3 lhs, Vector3 rhs);
Vector3 operator-(Vector3 lhs, float rhs);
Vector3 operator*(Vector3 lhs, float rhs);
Vector3 operator/(Vector3 lhs, float rhs);