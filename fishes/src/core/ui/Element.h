/*
 * Element.h
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <core/math/AABB.h>
#include <core/math/Vector2.h>

#include "UIManager.h"

namespace ui {

// Forward
//
class EventInfo;


class Element {
public:
    inline Element(UIManager *manager);
    virtual ~Element();

    /**
     * @brief This method will be called when a new event occur for a particular
     *        Element (for example: MouseOver, MouseExit, MouseClick, etc)
     * @parma event     The event info
     */
    virtual void
    newEvent(const EventInfo& event) = 0;

    /**
     * @brief Helper method to check if a point is inside of this element
     */
    inline bool
    checkPointInside(const math::Vector2ui& point) const;

    /**
     * @brief Enable/disable the element (this will add / remove the element
     *        from the UIManager)
     */
    inline void
    activate(bool enable);

    /**
     * @brief Check if the element is activated or not
     */
    inline bool
    isActivated(void) const;

    ////////////////////////////////////////////////////////////////////////////
    //                          Element Movement                              //
    ////////////////////////////////////////////////////////////////////////////
    //

    /**
     * @brief Get/Set the AABB associated to this element (all the elements will
     *        be taken as AABB<unsigned int>).
     */
    inline void
    setAABB(const math::AABBui& aabb);
    inline const math::AABBui&
    aabb(void) const;

    /**
     * @brief Set/Get position (helper classes).
     *        The position we use is always the top left one (using opengl coords)
     */
    inline void
    setPosition(const math::Vector2ui& pos);
    inline const math::Vector2ui&
    position(void) const;

    /**
     * @brief Translate the element
     */
    inline void
    translate(const math::Vector2ui& t);


protected:
    // we set the UImanager to be friend
    friend class UIManager;

    math::AABBui mAABB;
    UIManager *mManager;
    bool mActivated;

};




// Inline impl
//
inline Element::Element(UIManager *manager) :
    mManager(manager)
,   mActivated(false)
{
    ASSERT(manager);
}

inline bool
Element::checkPointInside(const math::Vector2ui& point) const
{
    return mAABB.checkPointInside(point);
}

inline bool
Element::isActivated(void) const
{
    return mActivated;
}

inline void
Element::activate(bool enable)
{
    ASSERT(mManager);
    if (enable) {
        if (mActivated) {
            // do nothing
            return;
        }
        mManager->addMenu(this); // this method will set mActivated = true
    } else {
        // check if is activated
        if (!mActivated) {
            // do nothing
            return;
        }
        mManager->removeMenu(this); // this method will set mActivated = false
    }
}

inline void
Element::setAABB(const math::AABBui& aabb)
{
    if (isActivated()) {
        activate(false);
        mAABB = aabb;
        activate(true);
    } else {
        mAABB = aabb;
    }
}

inline const math::AABBui&
Element::aabb(void) const
{
    return mAABB;
}

inline void
Element::setPosition(const math::Vector2ui& pos)
{
    if (isActivated()) {
        activate(false);
        mAABB.setPosition(pos);
        activate(true);
    } else {
        mAABB.setPosition(pos);
    }
}
inline const math::Vector2ui&
Element::position(void) const
{
    return mAABB.tl;
}

inline void
Element::translate(const math::Vector2ui& t)
{
    if (isActivated()) {
        activate(false);
        mAABB.translate(t);
        activate(true);
    } else {
        mAABB.translate(t);
    }
}

}

#endif /* ELEMENT_H_ */
