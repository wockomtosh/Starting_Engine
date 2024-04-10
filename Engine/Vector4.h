#pragma once

class Vector2;
class Vector3;

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4();
	Vector4(float i_x, float i_y, float i_z, float i_w);

	//TODO: The default should be 1, right?
	Vector4(const Vector2& i_other, float i_z = 0.0f, float i_w = 0.0f);
	Vector4(const Vector3& i_other, float i_w = 0.0f);
	//copy constructor
	Vector4(const Vector4& other);

	float getMagnitude() const;
	Vector4 normalize() const;
	//Vector4 getVectorWithMagnitude(float magnitude) const;
	bool isWithinRangeOf(Vector4 target, float range) const;

	void print();

	float dot(Vector4 other);

	Vector4 operator+=(Vector4 other);
	Vector4 operator-=(Vector4 other);

	Vector4 operator*=(float other);
	Vector4 operator/=(float other);

	Vector4 operator-();

	bool operator!=(Vector4 other);
	bool operator==(Vector4 other);

	static const Vector4 NULL_VECTOR;
};

Vector4 operator+(Vector4 lhs, Vector4 rhs);
Vector4 operator+(Vector4 lhs, float rhs);
Vector4 operator-(Vector4 lhs, Vector4 rhs);
Vector4 operator-(Vector4 lhs, float rhs);
Vector4 operator*(Vector4 lhs, float rhs);
Vector4 operator/(Vector4 lhs, float rhs);