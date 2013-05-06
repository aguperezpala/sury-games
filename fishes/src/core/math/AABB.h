/*
 * AABB.h
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#ifndef AABB_H_
#define AABB_H_


#include <iostream>

#include "Vector2.h"

namespace math {

// Aligned box2D
//
template<typename _T = float>
class AABB {
public:
    Vector2<_T> tl;
    Vector2<_T> br;

    AABB(){};
    AABB(const Vector2<_T> &topLeft, const Vector2<_T> &bottomRight) :
        tl(topLeft)
    ,   br(bottomRight)
    {
    }

    AABB(_T tlx, _T tly, _T brx, _T bry) :
        tl(Vector2<_T>(tlx, tly))
    ,   br(Vector2<_T>(brx, bry))
    {
    }

    // ge the center point (position)
    inline void center(Vector2<_T> &center) const
    {
        center.x = (br.x - tl.x) * 0.5f;
        center.y = (br.y - tl.y) * 0.5f;
    }

    // check if a point is inside of the box
    inline bool checkPointInside(const Vector2<_T> &p) const
    {
        return p.x >= tl.x && p.x <= br.x && p.y >= tl.y && p.y <= br.y;
    }

    // translate the bounding box
    inline void translate(const Vector2<_T> &v)
    {
        tl += v;
        br += v;
    }

    inline void setSize(const _T x, const _T y)
    {
       br.x = tl.x + x;
       br.y = tl.y + y;
    }

    inline _T getHeight(void) const {return br.y - tl.y;}
    inline _T getWidth(void) const {return br.x - tl.x;}

    // set the AABB in position v
    inline void setPosition(const Vector2<_T> &v)
    {
        const _T w = getWidth();
        const _T h = getHeight();
        tl = v;
        br.x = tl.x + w;
        br.y = tl.y + h;
    }

    // check the collision
    inline bool collide(const AABB &o) const
    {
        return !((o.br.x < tl.x) || (o.tl.x > br.x) || (o.br.y < tl.y) ||
                (br.y < o.tl.y));
    }

    // compare operators
    //
    inline bool operator==(const AABB<_T> &o) const
    {
        return tl == o.tl && br == o.br;
    }

    inline friend std::ostream& operator<<(std::ostream& o, const AABB<_T>& aabb)
    {
        o << "AABB(tl:" << aabb.tl << ", br: " << aabb.br << ")" << std::endl;
        return o;
    }

};

// typedef the AABBf
typedef AABB<float> AABBf;
typedef AABB<unsigned int> AABBui;

}

#endif /* AABB_H_ */
