/*
 * SpacePartition.h
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#ifndef SPACEPARTITION_H_
#define SPACEPARTITION_H_

#include <math/AABB.h>
#include <math/Vector2.h>

#include "Defines.h"

namespace s_p {

class SpacePartition {

public:
    SpacePartition();
    ~SpacePartition();


    /**
     * @brief Builds the CollisionSpace
     *        We will use the normal coordinates system, so the x grows
     *        from left to right, and the y grows from down to up.
     *        We also assume that the level start at the (0,0) position and
     *        grows to the positive axis values
     *
     * @param ssX   The space size X of the "world"
     * @param ssY   The space size Y of the "world"
     * @param cnX   The number of cells used in X
     * @param cnY   The number of cells used in Y
     */
    bool build(const float ssX, const float ssY, const size_t cnX, const size_t cnY);

    /**
     * @brief Returns the level size and cells info
     */
    inline float worldXSize(void) const;
    inline float worldYSize(void) const;
    inline float cellYSize(void) const;
    inline float cellXSize(void) const;
    inline size_t numCellsX(void) const;
    inline size_t numCellsy(void) const;


    // Space Partition object handling functions
    //

    /**
     * @brief Add a object (the object wasn't added before)
     * @param obj   The object to be added
     */
    void addObject(const Object *obj);

    /**
     * @brief Removes an object from the SpacePartition. This will stop the
     *        collision detection of the object.
     *        Requires the object be in the SpacePartition
     * @param obj   The object to be removed
     */
    void removeObject(const Object *obj);

    /**
     * @brief Remove all the objects
     */
    void removeAllObjects(void);


    // Specific object handling functions
    //

    /**
     * @brief Translate a object from the actual position using the vector t
     * @param obj   The object to be translated (it should be in the Space
     *              Partition)
     * @param t     The translation vector
     */
    void translateObject(Object *obj, const math::Vector2f &t);

    /**
     * @brief Set the position of an specific object using a math::vector2
     * @param obj   The object to be re-positionated
     * @param pos   The position vector
     */
    void setObjectPosition(Object *obj, const math::Vector2f &pos);


    // Space Partition querys
    //

    /**
     * @brief Get all the intersection for a certain object
     * @param obj       The object to be cheked against
     * @param result    The resulting vector to put the objects
     */
    void getIntersections(const Object *obj, ConstObjectVec &objs);

    /**
     * @brief Get all the objects that are in a AABB region (AABB query).
     * @param   aabb    The aabb region to be used
     * @param   mask    MaskGroup used to filter all the objects
     * @param   result  The Objects that are in the AABB
     */
    void getObjectsQuery(const math::AABBf &aabb,
                         uint32_t mask,
                         ConstObjectVec &result);

    /**
     * @brief Get all the objects that intersect a certain point with a certain
     *        mask (Point query)
     * @param point     The point to check
     * @param result    The result of the check
     * @param mask      The mask to be used
     */
    void getObjectsQuery(const math::Vector2f &point,
                         ConstObjectVec &result,
                         uint32_t mask = ~0u);

    /**
     * @brief Get all the objects that intersect a certain line segment with a
     *        certain mask (Line query)
     * @param p1        The first point of the line segment
     * @param p2        The second point of the line segment
     * @param result    The result of the check
     * @param mask      The mask to be used
     */
    void getObjectsQuery(const math::Vector2f &p1,
                         const math::Vector2f &p2,
                         ConstObjectVec &result,
                         uint32_t mask = ~0u);

    /**
     * @brief Function used to check if a point is inside the Space Partition
     * @param   p       The point to be checked
     * @return  True    if p is inside the Space Partition
     *          False   otherwise
     */
    inline bool isPointInside(const math::Vector2f &p);

private:
    float mFactorX;
    float mFactorY;
    float mCellSizeX;
    float mCellSizeY;
    float mXBounds;
    float mYBounds;
    size_t mNumCellX;
    size_t mNumCellY;
};



// Inline Implementations
//


inline float
SpacePartition::worldXSize(void) const
{
    return mCellSizeX * mNumCellX;
}
inline float
SpacePartition::worldYSize(void) const
{
    return mCellSizeY*mNumCellY;
}
inline float
SpacePartition::cellYSize(void) const
{
    return mCellSizeY;
}
inline float
SpacePartition::cellXSize(void) const
{
    return mCellSizeX;
}
inline size_t
SpacePartition::numCellsX(void) const
{
    return mNumCellX;
}
inline size_t
SpacePartition::numCellsy(void) const
{
    return mNumCellY;
}

////////////////////////////////////////////////////////////////////////////////
inline bool
SpacePartition::isPointInside(const math::Vector2f &p)
{
    if ((p.x < 0.0f || p.x > mXBounds) ||
            (p.y < 0.0f || p.y > mYBounds)){
        return false;
    }
    return true;
}



}

#endif /* SPACEPARTITION_H_ */
/
