/*
 * SceneManager.h
 *
 *  Created on: Feb 26, 2013
 *      Author: agustin
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <vector>
#include <iterator>

#include <SFML/Graphics/RenderWindow.hpp>

#include <common/debug/DebugUtil.h>
#include <core/math/AABB.h>

#include "LayerLevel.h"
#include "Entity.h"



// forward decl
//
namespace sys {
class GlobalConfig;
}

namespace scene {

class SceneManager
{
public:
    /**
     * @brief Constructor that takes the current configuration of the window
     * @param configWin The current configuration of the system used to take
     *                  the window size
     */
    SceneManager(sys::GlobalConfig& configWin);
    ~SceneManager();

    /**
     * @brief Add/remove an entity to be drawn in the sceneManager
     * @param entity    The entity itself
     */
    void
    addEntity(const Entity* entity);
    void
    removeEntity(const Entity* entity);

    /**
     * @brief Check if we already have an entity
     * @returns true if we have the entity | false otherwise
     */
    inline bool
    hasEntity(const Entity* entity) const;


    /**
     * @brief Draw all the entities associated to this SceneManager
     */
    void
    drawEntities(void);

private:



private:
    typedef std::vector<const Entity *> EntityVec;

    math::AABB<unsigned int> mCurrentScreen;
    EntityVec mEntities[LayerLevel::SIZE];
    sf::RenderWindow& mRenderWindow;

};


// Inline implementations
//
inline bool
SceneManager::hasEntity(const Entity* entity) const
{
    ASSERT(entity);
    ASSERT(entity->layerLevel() < LayerLevel::SIZE);
    const EntityVec& vec = mEntities[entity->layerLevel()];
    return entity->mID < vec.size() && vec[entity->mID] == entity;
}

} /* namespace scene */
#endif /* SCENEMANAGER_H_ */
