/*
 * AABB.h
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#ifndef AABB_H_
#define AABB_H_

#include "Vector2.h"

namespace math {

// Aligned box2D
//
template<typename _T>
class AABB {
public:
    Vector2<_T> tl;
    Vector2<_T> br;
    Vector2<_T> pos;

    AABB(){};
    AABB(const Vector2<_T> &topLeft, const Vector2<_T> &bottomRight)
    {
        tl = topLeft;
        br = bottomRight;
        pos = (tl + br) * 0.5f;
    }

    // check if a point is inside of the box
    inline bool checkPointInside(const Vector2<_T> &p) const
    {
        return p.x >= tl.x && p.x <= br.x && p.y >= br.y && p.y <= tl.y;
    }

    // translate the bounding box
    inline void translate(const Vector2<_T> &v)
    {
        tl += v;
        br += v;
        pos += v;
    }

    inline void setSize(const _T x, const _T y)
    {
        // init at the (0,0) pos
        tl.x = 0; tl.y = y;
        br.x = x; br.y = 0;
        pos.x = x *0.5f;
        pos.y = y *0.5f;
    }

    inline _T getHeight(void) const {return tl.y - br.y;}
    inline _T getWidth(void) const {return br.x - tl.x;}

    // set the AABB in position v as center
    inline void setPosition(const Vector2<_T> &v)
    {
        _T aux = (br.x - tl.x) * 0.5f;
        tl.x = v.x - aux; br.x = v.x + aux;
        aux = (tl.y - br.y) * 0.5f;
        tl.y = aux + v.y; br.y = v.y - aux;
        pos = v;
    }

    // check the collision
    inline bool collide(const AABB &o) const
    {
        return !((o.br.x < tl.x) || (o.tl.x > br.x) || (o.tl.y < br.y) ||
                (tl.y < o.br.y));
    }
};

// typedef the AABBf
typedef AABB<float> AABBf;

}

#endif /* AABB_H_ */
