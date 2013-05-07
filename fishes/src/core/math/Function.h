/*
 * Function.h
 *
 *  Created on: May 6, 2013
 *      Author: agustin
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <cmath>
#include <memory>

#include <common/debug/DebugUtil.h>

#include "Vector2.h"

namespace math {

////////////////////////////////////////////////////////////////////////////////
//                  Bezier quadratic helper method                            //
////////////////////////////////////////////////////////////////////////////////
//
struct BezierQuadratic {
    BezierQuadratic(){};
    BezierQuadratic(const math::Vector2f& aP1,
                    const math::Vector2f& aP2,
                    const math::Vector2f& aP3,
                    const math::Vector2f& aP4) :
        p1(aP1)
    ,   p2(aP2)
    ,   p3(aP3)
    ,   p4(aP4)
    {}

    // The four points that define the curve
    //
    math::Vector2f p1;
    math::Vector2f p2;
    math::Vector2f p3;
    math::Vector2f p4;

    math::Vector2f
    evaluate(float t) const
    {
        ASSERT(t >= 0.f && t <= 1.f);

        const float tneg = 1.0f - t;
        const float tneg2 = tneg * tneg;
        const float tneg3 = tneg2 * tneg;
        const float t2 = t * t;
        const float t3 = t2 * t;

        return math::Vector2f(
            tneg3 * p1.x + 3 * t * tneg2 * p2.x + 3 * tneg * t2 * p3.x + t3 * p4.x,

            tneg3 * p1.y + 3 * t * tneg2 * p2.y + 3 * tneg * t2 * p3.y + t3 * p4.y);
    }
};

////////////////////////////////////////////////////////////////////////////////
//                  Single interface for functions                            //
////////////////////////////////////////////////////////////////////////////////
//
class SingleFunction {
public:
    virtual ~SingleFunction(){};
    virtual float operator()(float) = 0;
};

// typedef of shared ptr
//
typedef std::shared_ptr<SingleFunction> SingleFunctionPtr;

// Linear function y(x) = a + b*x
//
class LinealFun : public SingleFunction {
public:
    LinealFun(float a, float b) : mA(a), mB(b) {}
    virtual ~LinealFun() {}
    float
    operator()(float x)
    {
        return mA + mB * x;
    }
private:
    float mA;
    float mB;
};

// Gauss approximation method
//
class GaussFun : public SingleFunction {
public:
    virtual ~GaussFun() {}
    float
    operator()(float x)
    {
        return (std::sin(x * 3.1415f - (3.1415f * 0.5f)) + 1.0f) * .5f;
    }
};


// Bezier quadratic but only for the y value (domain: [0,1] and image: [0,1]
//
class BezierQuadLineal : public SingleFunction {
public:
    BezierQuadLineal(const math::Vector2f& aP1,
                     const math::Vector2f& aP2,
                     const math::Vector2f& aP3,
                     const math::Vector2f& aP4) :
         mBezier(aP1, aP2, aP3, aP4)
    {}

    virtual ~BezierQuadLineal() {}
    float
    operator()(float x)
    {
        ASSERT(x >= 0 && x <= 1.0f);
        return mBezier.evaluate(x).y;
    }

private:
    BezierQuadratic mBezier;
};

////////////////////////////////////////////////////////////////////////////////
//          Vector functions will return a vector given a value X             //
////////////////////////////////////////////////////////////////////////////////
//
template<typename T>
class VectorFunction {
public:
    virtual ~VectorFunction(){};
    virtual math::Vector2<T> operator()(float) = 0;
};

// Translate vector method using bezier quadratic
// As precondition we need that the function used maps:
// 0 -> 0 and 1 -> 1
class TranslateVec : public VectorFunction<float> {
public:
    TranslateVec(const SingleFunctionPtr& fun) : mFun(fun)
    {
        // ensure that the function we will use will give us the values we want
        // between [0,1]. And will map: 0 -> 0 and 1 -> 1
        ASSERT(fun(0.f) == 0.f);
        ASSERT(fun(1.f) == 1.f);
    }
    virtual ~TranslateVec(){};

    // configure begin pos / end pos / time
    void
    configure(const math::Vector2f& begPos,
              const math::Vector2f& endPos,
              float time)
    {
        mBegPos = begPos;
        mDirPos = endPos;
        mDirPos -= begPos;
        mDirPos.normalize();
        mTime = time;
        mAccumTime = 0.0f;
    }

    // Call this method with the current time frame
    //
    math::Vector2f
    operator()(float x)
    {
        ASSERT(!finish());
        ASSERT(x > 0.f);
        ASSERT(mAccumTime >= 0.f);

        // transform the current accumulated time into t [0,1] to be passed to
        // the bezier curve
        mAccumTime += x;
        const float t = (mAccumTime < mTime) ? mAccumTime / mTime : 1.f;

        // get the y value and update the vector with that value
        const float yVal = mFun(t);
        ASSERT(yVal <= 1.f);
        return (mDirPos * yVal) + mBegPos;
    }

    // Check if the current method already finish
    //
    bool
    finish(void) const
    {
        return mAccumTime >= mTime;
    }

private:
    SingleFunctionPtr mFun;
    math::Vector2f mBegPos;
    math::Vector2f mDirPos;
    float mTime;
    float mAccumTime;
};

}


#endif /* FUNCTION_H_ */
