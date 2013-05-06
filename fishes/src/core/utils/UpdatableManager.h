/*
 * UpdatableManager.h
 *
 *  Created on: 06/05/2013
 *      Author: Agustin
 */

#ifndef UPDATABLEMANAGER_H_
#define UPDATABLEMANAGER_H_

#include <vector>
#include <cstddef>

#include <common/debug/DebugUtil.h>

#include "UpdatableObject.h"

namespace utils {

class UpdatableManager
{
public:
    UpdatableManager();
    ~UpdatableManager();

    /**
     * @brief Add an element to this manager to be updated
     * @param element       The element to be added
     * @note If the element already exists, it will be ignored.
     *       This class doesn't handle the memory
     */
    inline void
    addElement(UpdatableObject* element);

    /**
     * @brief Remove an existent element
     * @param element The element to be removed (if the element doesn't exists
     *                we ignore this call)
     */
    inline void
    removeElement(UpdatableObject* element);

    /**
     * @brief Check for the existence of an element
     */
    inline bool
    exists(UpdatableObject* element) const;

    /**
     * @brief Update all the elements we have
     */
    inline void
    update(void);

private:
    typedef std::vector<UpdatableObject*> ObjectsVec;

    ObjectsVec mObjects;

};

// Inline impl
//

inline void
UpdatableManager::addElement(UpdatableObject* element)
{
    ASSERT(element);
    if (exists(element)) {
        // nothing to do
        return;
    }
    // add the element
    element->id = mObjects.size();
    mObjects.push_back(element);
}

inline void
UpdatableManager::removeElement(UpdatableObject* element)
{
    ASSERT(element);
    if (!exists(element)) {
        return; // nothing to do
    }

    // remove it from the vector
    mObjects[element->id] = mObjects.back();
    mObjects.pop_back();
}

/**
 * @brief Check for the existence of an element
 */
inline bool
UpdatableManager::exists(UpdatableObject* element) const
{
    ASSERT(element);
    return element->id < mObjects.size() && mObjects[element->id] == element;
}


inline void
UpdatableManager::update(void)
{
    for (std::size_t i = 0, size = mObjects.size(); i < size; ++i) {
        mObjects[i]->update();
    }
}

} /* namespace utils */
#endif /* UPDATABLEMANAGER_H_ */
