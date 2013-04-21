/*
 * Resource.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: agustin
 */

#include "Resource.h"

namespace resources {

std::atomic<ResourceID> Resource::sIDCounter = 0;

Resource::Resource() :
    mID(sIDCounter++)
{
}


}
