#include "Matrix4.h"
#include <math.h>
#include <iostream>

Matrix4::Matrix4(
    float i_11, float i_12, float i_13, float i_14,
    float i_21, float i_22, float i_23, float i_24,
    float i_31, float i_32, float i_33, float i_34,
    float i_41, float i_42, float i_43, float i_44) :
    m_11(i_11), m_12(i_12), m_13(i_13), m_14(i_14),
    m_21(i_21), m_22(i_22), m_23(i_23), m_24(i_24),
    m_31(i_31), m_32(i_32), m_33(i_33), m_34(i_34),
    m_41(i_41), m_42(i_42), m_43(i_43), m_44(i_44) {}

Matrix4::Matrix4(const Matrix4& i_other) : 
    m_11(i_other.m_11), m_12(i_other.m_12), m_13(i_other.m_13), m_14(i_other.m_14),
    m_21(i_other.m_21), m_22(i_other.m_22), m_23(i_other.m_23), m_24(i_other.m_24),
    m_31(i_other.m_31), m_32(i_other.m_32), m_33(i_other.m_33), m_34(i_other.m_34),
    m_41(i_other.m_41), m_42(i_other.m_42), m_43(i_other.m_43), m_44(i_other.m_44) {}

Matrix4 Matrix4::operator*(const Matrix4& i_other) const
{
    float m11 = m_11 * i_other.m_11 + m_12 * i_other.m_21 + m_13 * i_other.m_31 + m_14 * i_other.m_41;
    float m12 = m_11 * i_other.m_12 + m_12 * i_other.m_22 + m_13 * i_other.m_32 + m_14 * i_other.m_42;
    float m13 = m_11 * i_other.m_13 + m_12 * i_other.m_23 + m_13 * i_other.m_33 + m_14 * i_other.m_43;
    float m14 = m_11 * i_other.m_14 + m_12 * i_other.m_24 + m_13 * i_other.m_34 + m_14 * i_other.m_44;

    float m21 = m_21 * i_other.m_11 + m_22 * i_other.m_21 + m_23 * i_other.m_31 + m_24 * i_other.m_41;
    float m22 = m_21 * i_other.m_12 + m_22 * i_other.m_22 + m_23 * i_other.m_32 + m_24 * i_other.m_42;
    float m23 = m_21 * i_other.m_13 + m_22 * i_other.m_23 + m_23 * i_other.m_33 + m_24 * i_other.m_43;
    float m24 = m_21 * i_other.m_14 + m_22 * i_other.m_24 + m_23 * i_other.m_34 + m_24 * i_other.m_44;

    float m31 = m_31 * i_other.m_11 + m_32 * i_other.m_21 + m_33 * i_other.m_31 + m_34 * i_other.m_41;
    float m32 = m_31 * i_other.m_12 + m_32 * i_other.m_22 + m_33 * i_other.m_32 + m_34 * i_other.m_42;
    float m33 = m_31 * i_other.m_13 + m_32 * i_other.m_23 + m_33 * i_other.m_33 + m_34 * i_other.m_43;
    float m34 = m_31 * i_other.m_14 + m_32 * i_other.m_24 + m_33 * i_other.m_34 + m_34 * i_other.m_44;

    float m41 = m_41 * i_other.m_11 + m_42 * i_other.m_21 + m_43 * i_other.m_31 + m_44 * i_other.m_41;
    float m42 = m_41 * i_other.m_12 + m_42 * i_other.m_22 + m_43 * i_other.m_32 + m_44 * i_other.m_42;
    float m43 = m_41 * i_other.m_13 + m_42 * i_other.m_23 + m_43 * i_other.m_33 + m_44 * i_other.m_43;
    float m44 = m_41 * i_other.m_14 + m_42 * i_other.m_24 + m_43 * i_other.m_34 + m_44 * i_other.m_44;

    return Matrix4(
        m11, m12, m13, m14,
        m21, m22, m23, m24,
        m31, m32, m33, m34,
        m41, m42, m43, m44);
}

Vector4 Matrix4::Row(size_t i_row) const
{
    switch (i_row)
    {
    case 1:
        return Vector4(m_11, m_12, m_13, m_14);
    case 2:
        return Vector4(m_21, m_22, m_23, m_24);
    case 3:
        return Vector4(m_31, m_32, m_33, m_34);
    case 4:
        return Vector4(m_41, m_42, m_43, m_44);
    }
    return Vector4();
}

Vector4 Matrix4::Col(size_t i_col) const
{
    switch (i_col)
    {
    case 1:
        return Vector4(m_11, m_21, m_31, m_41);
    case 2:
        return Vector4(m_12, m_22, m_32, m_42);
    case 3:
        return Vector4(m_13, m_23, m_33, m_43);
    case 4:
        return Vector4(m_14, m_24, m_34, m_44);
    }
    return Vector4();
}

Matrix4 Matrix4::CreateIdentity(void)
{
    return Matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

Matrix4 Matrix4::CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ)
{
    return Matrix4(
        i_scaleX, 0,        0,        0,
        0,        i_scaleY, 0,        0,
        0,        0,        i_scaleZ, 0,
        0,        0,        0,        1);
}

Matrix4 Matrix4::CreateXRotation(float i_RotationRadians)
{
    return Matrix4(
        1, 0,                       0,                       0,
        0, cos(i_RotationRadians),  -sin(i_RotationRadians), 0,
        0, sin(i_RotationRadians),  cos(i_RotationRadians),  0,
        0, 0,                       0,                       1);
}

Matrix4 Matrix4::CreateYRotation(float i_RotationRadians)
{
    return Matrix4(
        cos(i_RotationRadians),  0, sin(i_RotationRadians),  0,
        0,                       1, 0,                       0,
        -sin(i_RotationRadians), 0, cos(i_RotationRadians),  0,
        0,                       0, 0,                       1);
}

Matrix4 Matrix4::CreateZRotation(float i_RotationRadians)
{
    return Matrix4(
        cos(i_RotationRadians), -sin(i_RotationRadians), 0, 0,
        sin(i_RotationRadians),  cos(i_RotationRadians), 0, 0,
        0,                                            0, 1, 0,
        0,                                            0, 0, 1);
}

Matrix4 Matrix4::CreateTranslation(float i_transX, float i_transY, float i_transZ)
{
    return Matrix4(
        1, 0, 0, i_transX,
        0, 1, 0, i_transY,
        0, 0, 1, i_transZ,
        0, 0, 0, 1);
}

//See https://semath.info/src/inverse-cofactor-ex4.html for where I found a breakdown of this formula
Matrix4 Matrix4::GetInverse() const
{
    //Get a 4x4 determinant
    float det1 = m_11 * ((m_22 * m_33 * m_44) + (m_23 * m_34 * m_42) + (m_24 * m_32 * m_43)
                       - (m_24 * m_33 * m_42) - (m_23 * m_32 * m_44) - (m_22 * m_34 * m_43));
    float det2 = m_21 * ((m_12 * m_33 * m_44) + (m_13 * m_34 * m_42) + (m_14 * m_32 * m_43)
                       - (m_14 * m_33 * m_42) - (m_13 * m_32 * m_44) - (m_12 * m_34 * m_43));
    float det3 = m_31 * ((m_12 * m_23 * m_44) + (m_13 * m_24 * m_42) + (m_14 * m_22 * m_43)
                       - (m_14 * m_23 * m_42) - (m_13 * m_22 * m_44) - (m_12 * m_24 * m_43));
    float det4 = m_41 * ((m_12 * m_23 * m_34) + (m_13 * m_24 * m_32) + (m_14 * m_22 * m_33)
                       - (m_14 * m_23 * m_32) - (m_13 * m_22 * m_34) - (m_12 * m_24 * m_33));
    float determinant = det1 - det2 + det3 - det4;

    //Get the values of the adjugate matrix.
    float adj11 = ((m_22 * m_33 * m_44) + (m_23 * m_34 * m_42) + (m_24 * m_32 * m_43)
        - (m_24 * m_33 * m_42) - (m_23 * m_32 * m_44) - (m_22 * m_34 * m_43));
    float adj12 = -1 * ((m_12 * m_33 * m_44) + (m_13 * m_34 * m_42) + (m_14 * m_32 * m_43)
        - (m_14 * m_33 * m_42) - (m_13 * m_32 * m_44) - (m_12 * m_34 * m_43));
    float adj13 = ((m_12 * m_23 * m_44) + (m_13 * m_24 * m_42) + (m_14 * m_22 * m_43)
        - (m_14 * m_23 * m_42) - (m_13 * m_22 * m_44) - (m_12 * m_24 * m_43));
    float adj14 = -1 * ((m_12 * m_23 * m_34) + (m_13 * m_24 * m_32) + (m_14 * m_22 * m_33)
        - (m_14 * m_23 * m_32) - (m_13 * m_22 * m_34) - (m_12 * m_24 * m_33));

    float adj21 = -1 * ((m_21 * m_33 * m_44) + (m_23 * m_34 * m_41) + (m_24 * m_31 * m_43)
        - (m_24 * m_33 * m_41) - (m_23 * m_31 * m_44) - (m_21 * m_34 * m_43));
    float adj22 = ((m_11 * m_33 * m_44) + (m_13 * m_34 * m_41) + (m_14 * m_31 * m_43)
        - (m_14 * m_33 * m_41) - (m_13 * m_31 * m_44) - (m_11 * m_34 * m_43));
    float adj23 = -1 * ((m_11 * m_23 * m_44) + (m_13 * m_24 * m_41) + (m_14 * m_21 * m_43)
        - (m_14 * m_23 * m_41) - (m_13 * m_21 * m_44) - (m_11 * m_24 * m_43));
    float adj24 = ((m_11 * m_23 * m_34) + (m_13 * m_24 * m_31) + (m_14 * m_21 * m_33)
        - (m_14 * m_23 * m_31) - (m_13 * m_21 * m_34) - (m_11 * m_24 * m_33));

    float adj31 = ((m_21 * m_32 * m_44) + (m_22 * m_34 * m_41) + (m_24 * m_31 * m_42)
        - (m_24 * m_32 * m_41) - (m_22 * m_31 * m_44) - (m_21 * m_34 * m_42));
    float adj32 = -1 * ((m_11 * m_32 * m_44) + (m_12 * m_34 * m_41) + (m_14 * m_31 * m_42)
        - (m_14 * m_32 * m_41) - (m_12 * m_31 * m_44) - (m_11 * m_34 * m_42));
    float adj33 = ((m_11 * m_22 * m_44) + (m_12 * m_24 * m_41) + (m_14 * m_21 * m_42)
        - (m_14 * m_22 * m_41) - (m_12 * m_21 * m_44) - (m_11 * m_24 * m_42));
    float adj34 = -1 * ((m_11 * m_22 * m_34) + (m_12 * m_24 * m_31) + (m_14 * m_21 * m_32)
        - (m_14 * m_22 * m_31) - (m_12 * m_21 * m_34) - (m_11 * m_24 * m_32));

    float adj41 = -1 * ((m_21 * m_32 * m_43) + (m_22 * m_33 * m_41) + (m_23 * m_31 * m_42)
        - (m_23 * m_32 * m_41) - (m_22 * m_31 * m_43) - (m_21 * m_33 * m_42));
    float adj42 = ((m_11 * m_32 * m_43) + (m_12 * m_33 * m_41) + (m_13 * m_31 * m_42)
        - (m_13 * m_32 * m_41) - (m_12 * m_31 * m_43) - (m_11 * m_33 * m_42));
    float adj43 = -1 * ((m_11 * m_22 * m_43) + (m_12 * m_23 * m_41) + (m_13 * m_21 * m_42)
        - (m_13 * m_22 * m_41) - (m_12 * m_21 * m_43) - (m_11 * m_23 * m_42));
    float adj44 = ((m_11 * m_22 * m_33) + (m_12 * m_23 * m_31) + (m_13 * m_21 * m_32)
        - (m_13 * m_22 * m_31) - (m_12 * m_21 * m_33) - (m_11 * m_23 * m_32));

    //Divide each value by the determinant to get the final value
    return Matrix4(
        adj11 / determinant, adj12 / determinant, adj13 / determinant, adj14 / determinant,
        adj21 / determinant, adj22 / determinant, adj23 / determinant, adj24 / determinant,
        adj31 / determinant, adj32 / determinant, adj33 / determinant, adj34 / determinant,
        adj41 / determinant, adj42 / determinant, adj43 / determinant, adj44 / determinant);
}

//See https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html for a breakdown of this
//This assumes column vectors, and does NOT work with scaling.
Matrix4 Matrix4::GetInverseRotTrans() const
{
    //Get the translation vector
    Vector4 translation = Col(4);
    //Get the rotation vectors
    Vector4 xRot = Col(1);
    Vector4 yRot = Col(2);
    Vector4 zRot = Col(3);

    //Get our new translation values by negating and dotting it with the rotations
    //If there is no rotation this will just give the negative translation
    Vector4 negT = -translation;
    float tx = negT.dot(xRot);
    float ty = negT.dot(yRot);
    float tz = negT.dot(zRot);

    return Matrix4(
        xRot.x, xRot.y, xRot.z, tx,
        yRot.x, yRot.y, yRot.z, ty,
        zRot.x, zRot.y, zRot.z, tz,
        0, 0, 0, 1);
}

Matrix4 Matrix4::GetTranspose() const
{
    return Matrix4(
        m_11, m_21, m_31, m_41,
        m_12, m_22, m_32, m_42,
        m_13, m_23, m_33, m_43,
        m_14, m_24, m_34, m_44);
}

Vector4 Matrix4::MultiplyRight(const Vector4& i_vec) const
{
    float x = m_11 * i_vec.x + m_12 * i_vec.y + m_13 * i_vec.z + m_14 * i_vec.w;
    float y = m_21 * i_vec.x + m_22 * i_vec.y + m_23 * i_vec.z + m_24 * i_vec.w;
    float z = m_31 * i_vec.x + m_32 * i_vec.y + m_33 * i_vec.z + m_34 * i_vec.w;
    float w = m_41 * i_vec.x + m_42 * i_vec.y + m_43 * i_vec.z + m_44 * i_vec.w;
    return Vector4(x, y, z, w);
}

Vector4 Matrix4::MultiplyLeft(const Vector4& i_vec) const
{
    float x = m_11 * i_vec.x + m_21 * i_vec.y + m_31 * i_vec.z + m_41 * i_vec.w;
    float y = m_12 * i_vec.x + m_22 * i_vec.y + m_32 * i_vec.z + m_42 * i_vec.w;
    float z = m_13 * i_vec.x + m_23 * i_vec.y + m_33 * i_vec.z + m_43 * i_vec.w;
    float w = m_14 * i_vec.x + m_24 * i_vec.y + m_34 * i_vec.z + m_44 * i_vec.w;
    return Vector4(x, y, z, w);
}

void Matrix4::print()
{
    for (int i = 1; i <= 4; i++)
    {
        Row(i).print();
        std::cout << std::endl;
    }
}