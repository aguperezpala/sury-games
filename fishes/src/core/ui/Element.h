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

namespace ui {

// Forward
//
class EventInfo;


class Element {
public:
    Element();
    virtual ~Element();

    /**
     * @brief This method will be called when a new event occur for a particular
     *        Element (for example: MouseOver, MouseExit, MouseClick, etc)
     * @parma event     The event info
     */
    virtual void
    newEvent(const EventInfo& event) = 0;


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
    math::AABBui mAABB;
};




// Inline impl
//
inline void
Element::setAABB(const math::AABBui& aabb)
{
    mAABB = aabb;
}

inline const math::AABBui&
Element::aabb(void) const
{
    return mAABB;
}

inline void
Element::setPosition(const math::Vector2ui& pos)
{
    mAABB.setPosition(pos);
}
inline const math::Vector2ui&
Element::position(void) const
{
    return mAABB.tl;
}

inline void
Element::translate(const math::Vector2ui& t)
{
    mAABB.translate(t);
}

}

#endif /* ELEMENT_H_ */
