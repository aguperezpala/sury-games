/*
 * Vector2.h
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

namespace math {

template<typename _T>
static inline _T
InvSqrt(_T x){
    const _T xhalf = 0.5f*x;
    int i = *(int*)&x; // get bits for floating value
    i = 0x5f3759df - (i>>1); // gives initial guess y0
    x = *(_T*)&i; // convert bits back to float
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    return x;
}

template<typename _T>
class Vector2 {
public:
    _T x;
    _T y;
public:
    Vector2(){};
    Vector2(_T aX, _T aY) : x(aX), y(aY) {};
    Vector2(const Vector2 &o) : x(o.x), y(o.y) {};

    inline Vector2& operator =(const Vector2& rkVector) {
        x = rkVector.x;
        y = rkVector.y;
        return *this;
    }

    inline _T crossProduct(const Vector2 &other) const
    {
        return x*other.y - other.x * y;
    }

    inline _T squaredDistance(const Vector2 &other) const
    {
        const _T p = x - other.x;
        const _T q = y - other.y;
        return p*p + q*q;
    }

    inline void transformToNormal(void)
    {
        const _T aux = x;
        x = -y;
        y = aux;
    }

    inline void rotate180(void) {
        x = -x;
        y = -y;
    }

    // returns true if other is clockwise from this.
    inline bool isClockwise(const Vector2 &other) const
    {
        return crossProduct(other) < 0.0f;
    }

    inline Vector2& operator =(const _T fScalar) {
        x = fScalar;
        y = fScalar;

        return *this;
    }

    inline _T length(void) const {
        return 1.0f/InvSqrt(x*x + y*y);
    }

    inline _T squaredLength(void) const {
        return x*x + y*y;
    }

    inline _T inverseLen(void) const {
        return InvSqrt(x*x + y*y);
    }

    inline void normalize(void) {
        *this *= InvSqrt(x*x + y*y);
    }

    inline bool operator ==(const Vector2& rkVector) const {
        return (x == rkVector.x && y == rkVector.y);
    }

    inline bool operator !=(const Vector2& rkVector) const {
        return (x != rkVector.x || y != rkVector.y);
    }

    // arithmetic operations
    inline Vector2 operator +(const Vector2& rkVector) const {
        return Vector2(x + rkVector.x, y + rkVector.y);
    }

    inline Vector2 operator -(const Vector2& rkVector) const {
        return Vector2(x - rkVector.x, y - rkVector.y);
    }

    inline void operator -=(const Vector2& rkVector) {
        x -= rkVector.x; y -= rkVector.y;
    }

    inline void operator +=(const Vector2& rkVector) {
        x += rkVector.x; y += rkVector.y;
    }

    inline void operator *=(const _T fScalar) {
        x *= fScalar;
        y *= fScalar;
    }

    inline Vector2 operator *(const _T fScalar) const {
        return Vector2(x * fScalar, y * fScalar);
    }


    inline Vector2 operator *(const Vector2& rhs) const {
        return Vector2(x * rhs.x, y * rhs.y);
    }

    inline _T dotProduct(const Vector2 &o) const {
        return x * o.x + y * o.y;
    }

    inline void operator /=(const _T fScalar) {

        const _T fInv = 1.0f / fScalar;
        x *= fInv;
        y *= fInv;
    }

    inline Vector2 operator /(const Vector2& rhs) const {
        return Vector2(x / rhs.x, y / rhs.y);
    }

    inline Vector2 rotaitXDegrees (const _T rotM[][2]) const {
        return Vector2( x * rotM[0][0] + y * rotM[1][0],
                      x * rotM[0][1] + y * rotM[1][1]);
    }

};

// define the Vector2f
typedef Vector2<float> Vector2f;

}

#endif /* VECTOR2_H_ */
