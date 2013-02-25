/*
 * Matrix4.h
 *
 *  Created on: Feb 24, 2013
 *      Author: agustin
 */

#ifndef MATRIX4_H_
#define MATRIX4_H_

#include <debug/DebugUtil.h>

#include "Vector2.h"
#include "AABB.h"

namespace math {

class Matrix4 {
public:
    inline Matrix4(){};
    inline ~Matrix4(){};

    /** Default constructor.
        @note
            It does <b>NOT</b> initialize the matrix for efficiency.
    */
    inline Matrix4()
    {
    }

    inline Matrix4(float a00, float a01, float a02,
            float a10, float a11, float a12,
            float a20, float a21, float a22);


    // Constructor from a list of elements
    inline Matrix4(float m00, float m01, float m02, float m03,
                   float m10, float m11, float m12, float m13,
                   float m20, float m21, float m22, float m23,
                   float m30, float m31, float m32, float m33 );

    // Construct from a vector of elements
    inline Matrix4(const float *m)

    // copy constructor
    inline Matrix4(const Matrix4& m4x4);

    // return the Row of the matrix
    inline float* operator[](size_t iRow);
    inline const float *operator[](size_t iRow) const;

    // Combine matrices and return the reference of this one
    inline Matrix4 &combine(const Matrix4 &other);


    // Matrix concatenation (combine) using '*'.
    inline Matrix4 operator*( const Matrix4 &m2 ) const;
    inline Matrix4 &operator*=(const Matrix4 &m2 ) const;

    // Transform a point
    inline Vector2f transformPoint(const Vector2f& point) const;
    inline Vector2f transformPoint(float x, float y) const;
    inline Vector2f operator*(const Vector2f& point) const;
    inline Vector2f operator*(float x, float y) const;


    // Tests 2 matrices for equality / inequality.
    inline bool operator==(const Matrix4& m2) const;
    inline bool operator!=(const Matrix4& m2) const;

    // assignment operator
    inline Matrix4 &operator=(const Matrix4& m);

    // Multiply by scalar
    inline Matrix4 operator*(float scalar) const;
    inline Matrix4& operator*=(float scalar) const;

    // ---------------------------------------------------------
    // Translation / Rotation / Scale operations
    // ---------------------------------------------------------

    // set/get translation
    inline void setTranslation(const Vector2f& v);
    inline void setTranslation(float x, float y);
    inline void translate(const Vector2f& v);
    inline void translate(float x, float y);
    inline Vector2f getTranslation(void) const;

    // get translation matrix
    inline static Matrix4 getTranslationMat(const Vector2f& v);
    inline static Matrix4 getTranslationMat(float x, float y);

    // Set get scale
//    inline void setScale(const Vector2f& v);
//    inline void setScale(float x, float y);
//    inline Vector2f getScale(void);

    // Rotation operations
//    inline Matrix4 &rotate(float angle);

    // get the inverse matrix
    inline Matrix4 getInverse(void) const;

    // Transform an AABB using this matrix
    inline void transformAABB(AABBf &aabb) const;



    static const Matrix4 ZERO;
    static const Matrix4 IDENTITY;

private:
    float mM[16];
};


// Inline implementations
//

inline Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                        float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33 )
{
    mM[0] = m00;
    mM[1] = m01;
    mM[2] = m02;
    mM[3] = m03;
    mM[4] = m10;
    mM[5] = m11;
    mM[6] = m12;
    mM[7] = m13;
    mM[8] = m20;
    mM[9] = m21;
    mM[10] = m22;
    mM[11] = m23;
    mM[12] = m30;
    mM[13] = m31;
    mM[14] = m32;
    mv[15] = m33;
}

inline Matrix4::Matrix4(float a00, float a01, float a02,
                 float a10, float a11, float a12,
                 float a20, float a21, float a22)
{
    mM[0] = a00; mM[4] = a01; mM[8]  = 0.f; mM[12] = a02;
    mM[1] = a10; mM[5] = a11; mM[9]  = 0.f; mM[13] = a12;
    mM[2] = 0.f; mM[6] = 0.f; mM[10] = 1.f; mM[14] = 0.f;
    mM[3] = a20; mM[7] = a21; mM[11] = 0.f; mM[15] = a22;
}



inline Matrix4::Matrix4(const float *m)
{
    for (size_t i = 0; i < 16; ++i) {
        mM[i] = m[i];
    }
}


inline Matrix4::Matrix4(const Matrix4& m)
{
    for (size_t i = 0; i < 16; ++i) {
        mM[i] = m.mM[i];
    }
}


inline float*
Matrix4::operator[](size_t iRow)
{
    ASSERT(iRow < 4);
    return mM[iRow * 4];
}


inline const float *
Matrix4::operator[](size_t iRow) const
{
    ASSERT(iRow < 4);
    return mM[iRow * 4];
}


inline Matrix4 &
Matrix4::combine(const Matrix4 &b)
{
    // we are only using the 3 coordinates we want for 3 values
    mM[0] = mM[0] * b.mM[0]  + mM[4] * b.mM[1]  + mM[12] * b.mM[3];
    mM[1] = mM[1] * b.mM[0]  + mM[5] * b.mM[1]  + mM[13] * b.mM[3];
    mM[2] = 0.f;
    mM[3] = mM[3] * b.mM[0]  + mM[7] * b.mM[1]  + mM[15] * b.mM[3];
    mM[4] = mM[0] * b.mM[4]  + mM[4] * b.mM[5]  + mM[12] * b.mM[7];
    mM[5] = mM[1] * b.mM[4]  + mM[5] * b.mM[5]  + mM[13] * b.mM[7];
    mM[6] = 0.f;
    mM[7] = mM[3] * b.mM[4]  + mM[7] * b.mM[5]  + mM[15] * b.mM[7];
    mM[8]  = 0.f;
    mM[9]  = 0.f;
    mM[10]  = 1.f;
    mM[11]  = 0.f;
    mM[12] = mM[0] * b.mM[12] + mM[4] * b.mM[13] + mM[12] * b.mM[15];
    mM[13] = mM[1] * b.mM[12] + mM[5] * b.mM[13] + mM[13] * b.mM[15];
    mM[14]  = 0.f;
    mM[15] = mM[3] * b.mM[12] + mM[7] * b.mM[13] + mM[15] * b.mM[15];

    return *this;
}


inline Matrix4::Matrix4
Matrix4::operator*(const Matrix4 &m2) const
{
    Matrix4 r(*this);
    return r.combine(m2);
}


inline Matrix4::Matrix4 &
Matrix4::operator*=(const Matrix4 &m2)
{
    return combine(m2);
}


inline Vector2f
Matrix4::transformPoint(const Vector2f& point) const
{
    return Vector2f(mM[0] * point.x + mM[4] * point.y + mM[12],
                    mM[1] * point.x + mM[5] * point.y + mM[13]);
}


inline Vector2f
Matrix4::transformPoint(float x, float y) const
{
    return Vector2f(mM[0] * x + mM[4] * y + mM[12],
                    mM[1] * x + mM[5] * y + mM[13]);
}



inline Vector2f
Matrix4::operator*(const Vector2f& point) const
{
    return transformPoint(point);
}


inline Vector2f
Matrix4::operator*(float x, float y) const
{
    return transformPoint(x, y);
}


inline bool
Matrix4::operator==(const Matrix4& m2) const
{
    if(mM[0][0] != m2.mM[0][0] || mM[0][1] != m2.mM[0][1] ||
            mM[0][2] != m2.mM[0][2] || mM[0][3] != m2.mM[0][3] ||
            mM[1][0] != m2.mM[1][0] || mM[1][1] != m2.mM[1][1] ||
            mM[1][2] != m2.mM[1][2] || mM[1][3] != m2.mM[1][3] ||
            mM[2][0] != m2.mM[2][0] || mM[2][1] != m2.mM[2][1] ||
            mM[2][2] != m2.mM[2][2] || mM[2][3] != m2.mM[2][3] ||
            mM[3][0] != m2.mM[3][0] || mM[3][1] != m2.mM[3][1] ||
            mM[3][2] != m2.mM[3][2] || mM[3][3] != m2.mM[3][3] )
        return false;
    return true;
}

inline bool
Matrix4::operator!=(const Matrix4& m2) const
{
    return !(*this == m2);
}

inline Matrix4 &
Matrix4::operator=(const Matrix4& m)
{
    for(size_t i = 0; i < 16; ++i){
        mM[i] = m.mM[i];
    }
    return *this;
}

inline Matrix4
Matrix4::operator*(float scalar) const
{
    return Matrix4(mM[0] * scalar, mM[1] * scalar, mM[2] * scalar, mM[3] * scalar,
                   mM[4] * scalar, mM[5] * scalar, mM[6] * scalar, mM[7] * scalar,
                   mM[8] * scalar, mM[9] * scalar, mM[10] * scalar, mM[11] * scalar,
                   mM[12] * scalar, mM[13] * scalar, mM[14] * scalar, mM[15] * scalar);
}
inline Matrix4&
Matrix4::operator*=(float scalar) const
{
    for (size_t i = 0; i < 16; ++i){
        mM[i] *= scalar;
    }
    return *this;
}

// set/get translation
inline void
Matrix4::setTranslation(const Vector2f& v)
{
    mM[3] = v.x;
    mM[7] = v.y;
}
inline void
Matrix4::setTranslation(float x, float y)
{
    mM[3] = x;
    mM[7] = y;
}
inline void
Matrix4::translate(const Vector2f& v)
{
    combine(getTranslationMat(v));
}

inline void
Matrix4::translate(float x, float y)
{
    combine(getTranslationMat(x, y));
}


inline Vector2f
Matrix4::getTranslation(void) const
{
    return Vector2f(mM[3], mM[7]);
}

inline static Matrix4
Matrix4::getTranslationMat(const Vector2f& v)
{
    return Matrix4(1.0f, 0.0f, 0.0f, v.x,
                   0.0f, 1.0f, 0.0f, v.y,
                   0.0f, 0.0f, 1.0f, 0.f,
                   0.0f, 0.0f, 0.f, 1.0f);
}
inline static Matrix4
Matrix4::getTranslationMat(float x, float y)
{
    return Matrix4(1.0f, 0.0f, 0.0f, x,
                   0.0f, 1.0f, 0.0f, y,
                   0.0f, 0.0f, 1.0f, 0.f,
                   0.0f, 0.0f, 0.f, 1.0f);
}


// Set get scale
//inline void
//Matrix4::setScale(const Vector2f& v)
//{
//
//}
//
//inline Vector2f
//Matrix4::getScale(void)
//{
//
//}


inline Matrix4
Matrix4::getInverse(void) const
{
    // Compute the determinant
    const float det = mM[0] * (mM[15] * mM[5] - mM[7] * mM[13]) -
            mM[1] * (mM[15] * mM[4] - mM[7] * mM[12]) +
            mM[3] * (mM[13] * mM[4] - mM[5] * mM[12]);

    // Compute the inverse if the determinant is not zero
    // (don't use an epsilon because the determinant may *really* be tiny)
    if (det != 0.f) {
        const float detFactor = 1.0f / det;
        return Matrix4( (mM[15] * mM[5] - mM[7] * mM[13]) * detFactor,
                         -(mM[15] * mM[4] - mM[7] * mM[12]) * detFactor,
                          (mM[13] * mM[4] - mM[5] * mM[12]) * detFactor,
                         -(mM[15] * mM[1] - mM[3] * mM[13]) * detFactor,
                          (mM[15] * mM[0] - mM[3] * mM[12]) * detFactor,
                         -(mM[13] * mM[0] - mM[1] * mM[12]) * detFactor,
                          (mM[7]  * mM[1] - mM[3] * mM[5])  * detFactor,
                         -(mM[7]  * mM[0] - mM[3] * mM[4])  * detFactor,
                          (mM[5]  * mM[0] - mM[1] * mM[4])  * detFactor);
    } else {
        return Matrix4::IDENTITY;
    }
}

inline void
Matrix4::transformAABB(AABBf &aabb) const
{
    // Transform the 4 corners of the rectangle
    const Vector2f points[] =
    {
        transformPoint(aabb.tl),
        transformPoint(aabb.tl.x, aabb.br.y),
        transformPoint(aabb.br.x, aabb.tl.y),
        transformPoint(aabb.br)
    };

    // Compute the bounding rectangle of the transformed points
    float left = points[0].x;
    float top = points[0].y;
    float right = points[0].x;
    float bottom = points[0].y;
    for (int i = 1; i < 4; ++i)
    {
        if      (points[i].x < left)   left = points[i].x;
        else if (points[i].x > right)  right = points[i].x;
        if      (points[i].y < top)    top = points[i].y;
        else if (points[i].y > bottom) bottom = points[i].y;
    }

    // we are using the inverse coordinate system
    aabb.tl.x = left;
    aabb.tl.y = bottom;
    aabb.br.x = right;
    aabb.br.y = top;
}


}

#endif /* MATRIX4_H_ */
