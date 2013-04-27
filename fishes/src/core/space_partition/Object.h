/*
 * Object.h
 *
 *  Created on: Feb 17, 2013
 *      Author: agustin
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <climits>
#include <cstdint>

#include <math/Vector2.h>
#include <math/AABB.h>


namespace s_p {

// Forward declarations
//
class TwoDimCell;
class SpacePartition;

class Object {
public:
    static const std::uint32_t INVALID_INDEX = UINT_MAX;
public:
    Object() :
        mGroupMask(~0u)
    ,   mUserDefined(0)
    ,   mID(INVALID_INDEX)
    {}

    /**
     * @brief Set/Get the group mask for this object (this is used to
     * differentiate "collisions groups").
     * @param groupMask     The mask to be used for this object
     */
    inline void setGroupMask(const uint32_t groupMask);
    inline uint32_t groupMask(void) const;

    /**
     * @brief Position query function
     * @param pos   The position where the object is
     */
    inline void position(math::Vector2<UnitType> &pos) const;

    /**
     * @brief User defined functions.
     */
    template <typename _T>
    inline void userDefined(_T *&result);
    inline const void *userDefined(void) const;
    inline void setUserDefined(void *o);

    /**
     * @brief Returns the AABB associated to this object
     * @returns the AABB of this object
     */
    inline const math::AABB<UnitType> &aabb(void) const;
    inline math::AABB<UnitType> &aabb(void);

    /**
     * @brief Set the AABB associated to this object
     * @param aabb  the new AABB for this object
     */
    inline void setAABB(const math::AABB<UnitType> &aabb);


private:
    // All this functions will be used by the Cell classes only
    friend class TwoDimCell;
    friend class SpacePartition;

    /**
     * @brief Position handling and query functions
     * @param pos   The position where we want to put the object
     */
    inline void setPosition(const math::Vector2<UnitType> &pos);


private:
    // mask object (for collision groups)
    std::uint32_t mGroupMask;
    // the associated AABB
    math::AABB<UnitType>  mAABB;
    // user defined type
    void *mUserDefined;
    // the id of the object (index in manager)
    std::uint32_t mID;
};



// Inline implementations
//

inline void
Object::setPosition(const math::Vector2<UnitType> &pos)
{
    mAABB.setPosition(pos);
}
inline void
Object::position(math::Vector2<UnitType> &pos) const
{
    mAABB.center(pos);
}

template <typename _T>
inline void
Object::userDefined(_T *&result)
{
    result = static_cast<_T *>(mUserDefined);
}
inline const void *
Object::userDefined(void) const
{
    return mUserDefined;
}
inline void
Object::setUserDefined(void *o)
{
    mUserDefined = o;
}

inline const math::AABB<UnitType> &
Object::aabb(void) const
{
    return mAABB;
}
inline math::AABB<UnitType> &
Object::aabb(void)
{
    return mAABB;
}
inline void
Object::setAABB(const math::AABB<UnitType> &aabb)
{
    mAABB = aabb;
}
}

#endif /* OBJECT_H_ */
