/*
 * Object.h
 *
 *  Created on: Feb 17, 2013
 *      Author: agustin
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <climits>

#include <math/Vector2.h>
#include <math/AABB.h>


namespace s_p {

// Forward declarations
//
class TwoDimCell;

class Object {
public:
    static const unsigned int INVALID_INDEX = UINT_MAX;
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
     * @return pos   The position where the object is
     */
    inline const math::Vector2f &position(void) const;

    /**
     * @brief User defined functions.
     */
    template <typename _T>
    inline void userDefined(_T *&result);
    inline const void *userDefined(void) const;
    inline void setUserDefined(void *o);


private:
    // All this functions will be used by the Cell classes only
    friend class TwoDimCell;

    /**
     * @brief Position handling and query functions
     * @param pos   The position where we want to put the object
     */
    inline void setPosition(const math::Vector2f &pos);


private:
    // mask object (for collision groups)
    uint32_t mGroupMask;
    // the associated AABB
    math::AABBf  mAABB;
    // user defined type
    void *mUserDefined;
    // the id of the object (index in manager) TODO: we are really using this?
    uint32_t mID;
};



// Inline implementations
//

inline void
Object::setPosition(const math::Vector2f &pos)
{
    mAABB.setPosition(pos);
}
inline const math::Vector2f &
Object::position(void) const
{
    mAABB.pos;
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

}

#endif /* OBJECT_H_ */
