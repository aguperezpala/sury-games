/*
 * Entity.h
 *
 *  Created on: Feb 24, 2013
 *      Author: agustin
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <math/AABB.h>
#include <common/debug/DebugUtil.h>

namespace scene {

class Entity {
public:
    Entity();
    ~Entity();

    /**
     * @brief Returns the bounding box of the entity
     * @param aabb  The bounding box to fill by the entity
     */
    inline void boundingBox(math::AABBf &aabb) const;
};


// Inline impl
//
inline void
Entity::boundingBox(math::AABBf &aabb) const
{
    ASSERT(false);
}

}

#endif /* ENTITY_H_ */
