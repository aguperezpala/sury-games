/*
 * Entity.h
 *
 *  Created on: Feb 24, 2013
 *      Author: agustin
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <math/AABB.h>

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

}

#endif /* ENTITY_H_ */
