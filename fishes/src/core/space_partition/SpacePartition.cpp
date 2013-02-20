/*
 * SpacePartition.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#include "SpacePartition.h"

namespace s_p {


SpacePartition::SpacePartition() :
        mFactorX(0)
,       mFactorY(0)
,       mCellSizeX(0)
,       mCellSizeY(0)
,       mXBounds(0)
,       mYBounds(0)
,       mNumCellX(0)
,       mNumCellY(0)
{

}

SpacePartition::~SpacePartition()
{

}

////////////////////////////////////////////////////////////////////////////////
bool
SpacePartition::build(const float ssX, const float ssY,
                      const size_t cnX, const size_t cnY)
{

}

// Space Partition object handling functions
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::addObject(const Object *obj);

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::removeObject(const Object *obj);

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::removeAllObjects(void);


// Specific object handling functions
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::translateObject(Object *obj, const math::Vector2f &t);

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::setObjectPosition(Object *obj, const math::Vector2f &pos);


// Space Partition querys
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getIntersections(const Object *obj, ConstObjectVec &objs);

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::AABBf &aabb,
                                uint32_t mask,
                                ConstObjectVec &result);

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::Vector2f &point,
                                ConstObjectVec &result,
                                uint32_t mask);

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::Vector2f &p1,
                                const math::Vector2f &p2,
                                ConstObjectVec &result,
                                uint32_t mask);

}
