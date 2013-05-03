/*
 * Element.cpp
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#include "Element.h"

namespace ui {


Element::~Element() {
    // TODO Auto-generated destructor stub
}

void
Element::elementChanged(void)
{
    debugWARNING("This element hasn't implemented the elementChanged() method\n");
}

}
