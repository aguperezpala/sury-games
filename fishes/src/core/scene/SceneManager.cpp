/*
 * SceneManager.cpp
 *
 *  Created on: Feb 26, 2013
 *      Author: agustin
 */

#include "SceneManager.h"

#include <set>

#include <core/system/GlobalConfig.h>



// Helper methods
//
namespace {
template<typename T>
inline void
swapWithLast(std::vector<T> &vec, size_t index)
{
    const size_t size = vec.size();
    vec[index] = vec[size-1];
    vec.pop_back();
}

}


namespace scene {


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager(sys::GlobalConfig& configWin) :
    mRenderWindow(configWin.renderWindow())
{
    mCurrentScreen.tl.x = mCurrentScreen.tl.y = 0;
    mCurrentScreen.br.x = configWin.windowWidth();
    mCurrentScreen.br.y = configWin.windowHeight();
}

////////////////////////////////////////////////////////////////////////////////
SceneManager::~SceneManager()
{
}

void
SceneManager::addEntity(const Entity* entity)
{
    ASSERT(entity);

    if (hasEntity(entity)) {
        debugWARNING("Trying to add two times the same entity\n");
        return;
    }
    mEntities[entity->layerLevel()].push_back(entity);
}

void
SceneManager::removeEntity(const Entity* entity)
{
    ASSERT(entity);
    if (!hasEntity(entity)) {
        debugWARNING("Trying to remove an inexistent entity\n");
        return;
    }
    swapWithLast(mEntities[entity->layerLevel()], entity->mID);
}

////////////////////////////////////////////////////////////////////////////////
void
SceneManager::drawEntities(void)
{
    // we will draw the entities in the order we need
    for (size_t i = 0; i < LayerLevel::SIZE; ++i) {
        EntityVec& vec = mEntities[i];
        for (size_t j = 0, size = vec.size(); j < size; ++j) {
            // we will update the queue of the entities if we need here
            mRenderWindow.draw(*vec[j]);
            if (vec[j]->layerLevel() != static_cast<LayerLevel>(i)) {
                // we need to update the queue of this entity it
                removeEntity(vec[j]);
                addEntity(vec[j]);
                // the size of this vector and the current possition has changed
                --j;
                --size;
            }
        }
    }
}

} /* namespace scene */
