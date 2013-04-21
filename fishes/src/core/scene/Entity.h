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
#include <ui/AnimatedSprite.h>
#include <SFML/Graphics/Sprite.hpp>

namespace scene {

class Entity {
public:
    Entity();
    ~Entity();

    // TEST: TODO: remove this
    ui::AnimatedSprite* animSprite;


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
    if (animSprite == 0) {
        return;
    }
    sf::FloatRect fr = animSprite->getGlobalBounds();
    aabb.tl.x = fr.left;
    aabb.tl.y = fr.top;
    aabb.br.x = fr.left + fr.width;
    aabb.br.y = fr.top + fr.height;
}

}

#endif /* ENTITY_H_ */
