#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <iostream>

Vector4::Vector4() : 
	x(0), y(0), z(0), w(0) {}

Vector4::Vector4(float i_x, float i_y, float i_z, float i_w) :
	x(i_x), y(i_y), z(i_z), w(i_w) {}

Vector4::Vector4(const Vector2& i_other, float i_z, float i_w) :
	x(i_other.x), y(i_other.y), z(i_z), w(i_w) {}

Vector4::Vector4(const Vector3& i_other, float i_w) :
	x(i_other.x), y(i_other.y), z(i_other.z), w(i_w) {}

Vector4::Vector4(const Vector4& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

float Vector4::getMagnitude() const
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
}

Vector4 Vector4::normalize() const
{
	float magnitude = getMagnitude();
	if (magnitude != 0)
	{
		return Vector4(x, y, z, w) / getMagnitude();
	}
	return Vector4();
}

bool Vector4::isWithinRangeOf(Vector4 target, float range) const
{
	bool withinXRange = x < (target.x + range) && x > (target.x - range);
	bool withinYRange = y < (target.y + range) && y > (target.y - range);
	bool withinZRange = z < (target.z + range) && z > (target.z - range);
	bool withinWRange = w < (target.w + range) && w > (target.w - range);
	return withinXRange && withinYRange && withinZRange && withinWRange;
}

void Vector4::print()
{
	std::cout << "[" << x << ", " << y << ", " << z << ", " << w << "]";
}

float Vector4::dot(Vector4 other)
{
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

Vector4 Vector4::operator+=(Vector4 other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator-=(Vector4 other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator*=(float other)
{
	x *= other;
	y *= other;
	z *= other;
	w *= other;
	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator/=(float other)
{
	x /= other;
	y /= other;
	z /= other;
	w /= other;
	return Vector4(x, y, z, w);
}

Vector4 Vector4::operator-()
{
	return Vector4(-x, -y, -z, -w);
}

bool Vector4::operator!=(Vector4 other)
{
	return !isWithinRangeOf(other, .0001);
}

bool Vector4::operator==(Vector4 other)
{
	return isWithinRangeOf(other, .0001);
}

Vector4 operator+(Vector4 lhs, Vector4 rhs)
{
	return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

Vector4 operator+(Vector4 lhs, float rhs)
{
	return Vector4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
}

Vector4 operator-(Vector4 lhs, Vector4 rhs)
{
	return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

Vector4 operator-(Vector4 lhs, float rhs)
{
	return Vector4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
}

Vector4 operator*(Vector4 lhs, float rhs)
{
	return Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

Vector4 operator/(Vector4 lhs, float rhs)
{
	return Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

const Vector4 Vector4::NULL_VECTOR = Vector4(-10000, -10000, -10000, -10000);