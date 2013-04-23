/*
 * Entity.h
 *
 *  Created on: Feb 24, 2013
 *      Author: agustin
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <SFML/Graphics/Sprite.hpp>

#include <math/AABB.h>
#include <common/debug/DebugUtil.h>

#include "LayerLevel.h"

namespace scene {

// Forward declaration
//
class SceneManager;

class Entity {

    // friend classes
    //
    friend class SceneManager;

public:
    inline Entity();
    inline ~Entity() {};

    /**
     * @brief Returns the bounding box of the entity
     * @param aabb  The bounding box to fill by the entity
     */
    inline void
    boundingBox(math::AABBf& aabb) const;

    /**
     * @brief Get/Set the layer level, lower layers will be drawn behind objects
     *        with higher layer levels (that will be drawn on top)
     */
    inline LayerLevel
    layerLevel(void) const;
    inline void
    setLayerLevel(LayerLevel ll);

    /**
     * @brief Get/Set an sprite to this entity (since sf::Sprite will be almost
     *        the only thing we want to display on the screen as "GameObjects"
     *        and automatically convert to a drawable object for convenience.
     */
    inline const sf::Sprite&
    sprite(void) const;
    inline sf::Sprite&
    sprite(void);

    inline void
    setSprite(const sf::Sprite&);
    inline operator const sf::Drawable&(void) const;

    /**
     * @brief Set/Get visibility flag of this entity
     */
    inline bool
    isVisible(void) const;
    inline void
    setVisible(bool);

private:
    LayerLevel mLayerLevel;
    sf::Sprite mSprite;
    bool mVisible;

    // id used by the sceneManager for a fast handle / render
    mutable size_t mID;
};


// Inline impl
//
inline Entity::Entity() :
    mLayerLevel(LayerLevel::LAYER_0)
{
}


inline void
Entity::boundingBox(math::AABBf& aabb) const
{
    sf::FloatRect fr = mSprite.getGlobalBounds();
    aabb.tl.x = fr.left;
    aabb.tl.y = fr.top;
    aabb.br.x = fr.left + fr.width;
    aabb.br.y = fr.top + fr.height;
}

inline LayerLevel
Entity::layerLevel(void) const
{
    return mLayerLevel;
}
inline void
Entity::setLayerLevel(LayerLevel ll)
{
    mLayerLevel = ll;
}

inline const sf::Sprite&
Entity::sprite(void) const
{
    return mSprite;
}
inline sf::Sprite&
Entity::sprite(void)
{
    return mSprite;
}

inline void
Entity::setSprite(const sf::Sprite& s)
{
    mSprite = s;
}

inline Entity::operator const sf::Drawable&(void) const
{
    return mSprite;
}

inline bool
Entity::isVisible(void) const
{
    return mVisible;
}
inline void
Entity::setVisible(bool v)
{
    mVisible = v;
}
}

#endif /* ENTITY_H_ */
