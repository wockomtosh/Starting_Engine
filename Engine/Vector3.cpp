#include "Vector3.h"
#include <iostream>

Vector3::Vector3() :
	x(0), y(0), z(0) {}

Vector3::Vector3(float i_x, float i_y, float i_z) :
	x(i_x), y(i_y), z(i_z) {}

Vector3::Vector3(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

float Vector3::getMagnitude() const
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3 Vector3::normalize() const
{
	float magnitude = getMagnitude();
	if (magnitude != 0)
	{
		return Vector3(x, y, z) / getMagnitude();
	}
	return Vector3();
}

bool Vector3::isWithinRangeOf(Vector3 target, float range) const
{
	bool withinXRange = x < (target.x + range) && x >(target.x - range);
	bool withinYRange = y < (target.y + range) && y >(target.y - range);
	bool withinZRange = z < (target.z + range) && z >(target.z - range);
	return withinXRange && withinYRange && withinZRange;
}

void Vector3::print()
{
	std::cout << "[" << x << ", " << y << ", " << z << "]";
}

float Vector3::dot(Vector3 other)
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::operator+=(Vector3 other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return Vector3(x, y, z);
}

Vector3 Vector3::operator-=(Vector3 other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return Vector3(x, y, z);
}

Vector3 Vector3::operator*=(float other)
{
	x *= other;
	y *= other;
	z *= other;
	return Vector3(x, y, z);
}

Vector3 Vector3::operator/=(float other)
{
	x /= other;
	y /= other;
	z /= other;
	return Vector3(x, y, z);
}

Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}

bool Vector3::operator!=(Vector3 other)
{
	return !isWithinRangeOf(other, .0001);
}

bool Vector3::operator==(Vector3 other)
{
	return isWithinRangeOf(other, .0001);
}

Vector3 operator+(Vector3 lhs, Vector3 rhs)
{
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vector3 operator+(Vector3 lhs, float rhs)
{
	return Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
}

Vector3 operator-(Vector3 lhs, Vector3 rhs)
{
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vector3 operator-(Vector3 lhs, float rhs)
{
	return Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
}

Vector3 operator*(Vector3 lhs, float rhs)
{
	return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vector3 operator/(Vector3 lhs, float rhs)
{
	return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

const Vector3 Vector3::NULL_VECTOR = Vector3(-10000, -10000, -10000);