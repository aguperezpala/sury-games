/*
 * SingleUpdater.h
 *
 *  Created on: 07/05/2013
 *      Author: Harriague
 */

#ifndef SingleUpdater_H_
#define SingleUpdater_H_


#include <vector>
#include <cstddef>

#include <common/debug/DebugUtil.h>

namespace common {

// Forward
//


class SingleUpdaterElement {
public:
    SingleUpdaterElement() : finish(false) {};
    virtual ~SingleUpdaterElement(){};

    /**
     * @brief Method called every frame to update the logic of this element
     */
    virtual void
    update(void) = 0;

    /**
     * @brief Method called when the element finish its update.
     */
    virtual void
    finished(void)
    {
        debugWARNING("Method not implemented\n");
    };

    /**
     * @brief Method called when the element is added to the manager and start
     *        to be updated
     */
    virtual void
    starting(void)
    {
        debugWARNING("Method not implemented\n");
    };

    /**
     * @brief Set/Get the state of the SingleUpdaterElement to see if already
     *        finish or not
     */
    inline bool
    hasFinish(void) const
    {
        return mFinish;
    }
    inline void
    setFinish(bool f)
    {
        mFinish = f;
    }

    /**
     * @brief Needed methods to be used for the manager. Don't call this methods
     *        directly
     */
    inline void
    setID(unsigned int id)
    {
        mID = id;
    }
    inline unsigned int
    id(void) const
    {
        return mID;
    }

private:
    unsigned int mID;
    bool mFinish;
};

// This is a template just in case we don't want to use SingleUpdater
//
template<typename T>
class SingleUpdater
{
public:
    SingleUpdater(){};
    ~SingleUpdater(){};

    /**
     * @brief Add an element to this manager to be updated
     * @param element       The element to be added
     * @note If the element already exists, it will be ignored.
     *       This class doesn't handle the memory
     */
    inline void
    add(T* element)
    {
        ASSERT(element);
        if (exists(element)) {
            // nothing to do
            return;
        }

        // call the initialize method of the element
        element->starting();

        // add the element
        element->setID(mObjects.size());
        element->setFinish(false);
        mObjects.push_back(element);
    }

    /**
     * @brief Remove an existent element
     * @param element The element to be removed (if the element doesn't exists
     *                we ignore this call)
     */
    inline void
    remove(T* element)
    {
        ASSERT(element);
        if (!exists(element)) {
            return; // nothing to do
        }

        // call the finished method
        element->finished();

        // remove it from the vector
        mObjects[element->id()] = mObjects.back();
        mObjects.pop_back();
    }

    /**
     * @brief Check for the existence of an element
     */
    inline bool
    exists(T* element) const
    {
        ASSERT(element);
        return element->id() < mObjects.size() && mObjects[element->id()] == element;
    }


    /**
     * @brief Update all the elements we have
     */
    inline void
    update(void)
    {
        for (std::size_t i = 0, size = mObjects.size(); i < size; ++i) {
            // check if the element already finish
            T* element = mObjects[i];
            if (element->hasFinish()) {
                // remove this element and continue with the next one
                element->finished();
                mObjects[i] = mObjects.back();
                mObjects[i]->setID(i);
                --i;
                --size;
            } else {
                element->update();
            }
        }
    }

private:
    typedef std::vector<T*> ObjectsVec;

    ObjectsVec mObjects;
};

}


#endif /* SingleUpdater_H_ */
