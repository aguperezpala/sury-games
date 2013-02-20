/*
 * Line.h
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#ifndef LINE_H_
#define LINE_H_

#include "Vector2.h"

namespace math {

// TODO
template<typename _T>
class Line {
public:
    Vector2<_T> p1;
    Vector2<_T> p2;
public:
    Line();
    ~Line();
};

// define Linef
//
typedef Line<float> Linef;

}

#endif /* LINE_H_ */
