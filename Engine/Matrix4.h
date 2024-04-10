#pragma once

#include "Vector4.h"
#include "Vector3.h"

//I like column vectors so I'm only supporting column vectors for the matrices that I create (transform, rotation, etc)
//I'll support row and column vectors for operations, but I won't be doing the extra work to set up row matrices.
class Matrix4 
{
private:
	//set up as m_[row][column]
	float
		m_11, m_12, m_13, m_14,
		m_21, m_22, m_23, m_24,
		m_31, m_32, m_33, m_34,
		m_41, m_42, m_43, m_44;

public:
	//Does nothing, since we'll usually initialize with other functions. But we need a default.
	Matrix4() {}
	
	Matrix4(
		float i_11, float i_12, float i_13, float i_14,
		float i_21, float i_22, float i_23, float i_24,
		float i_31, float i_32, float i_33, float i_34,
		float i_41, float i_42, float i_43, float i_44);

	//copy constructor
	Matrix4(const Matrix4& i_other);

	Matrix4 operator*(const Matrix4& other) const;

	//These were in the recommended .h file, I may want to implement them later but not right now.
	//Vector3 TransformNormalRV(const Vector3& i_Normal) const;
	//Vector3 TransformNormalCV(const Vector3& i_Normal) const;

	// Grab a particular row or column
	Vector4 Row(size_t i_row) const;
	Vector4 Col(size_t i_col) const;

	// Useful matrix creators
	static Matrix4 CreateIdentity(void);
	static Matrix4 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ);

	// Layout for column vector multiplies ( V = M * V )
	static Matrix4 CreateXRotation(float i_RotationRadians);
	static Matrix4 CreateYRotation(float i_RotationRadians);
	static Matrix4 CreateZRotation(float i_RotationRadians);

	static Matrix4 CreateTranslation(float i_transX, float i_transY, float i_transZ = 0.0f);
	inline static Matrix4 CreateTranslation(const Vector3& i_trans)
	{
		return CreateTranslation(i_trans.x, i_trans.y, i_trans.z);
	}

	Matrix4 GetInverse() const;

	//Inverse for matrices that only involve axis rotation or translation matrices
	Matrix4 GetInverseRotTrans() const;

	Matrix4 GetTranspose() const;

	Vector4 MultiplyLeft(const Vector4& i_Vector) const;
	Vector4 MultiplyRight(const Vector4& i_Vector) const;

	void print();

};

//Column vector multiplication
inline Vector4 operator*(const Matrix4 i_mtx, const Vector4 i_vec)
{
	return i_mtx.MultiplyRight(i_vec);
}

//Row vector multiplication
inline Vector4 operator*(const Vector4 i_vec, const Matrix4 i_mtx) 
{
	return i_mtx.MultiplyLeft(i_vec);
}