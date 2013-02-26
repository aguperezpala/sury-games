/*
 * SpacePartition.h
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#ifndef SPACEPARTITION_H_
#define SPACEPARTITION_H_

#include <vector>
#include <limits>

#include <math/AABB.h>
#include <math/Vector2.h>
#include <debug/DebugUtil.h>

#include "Defines.h"
#include "TwoDimCell.h"
#include "Object.h"

namespace s_p {


class SpacePartition {

    typedef std::vector<TwoDimCell> CellsVec;

    // Matrix definition
    class Matrix {
    public:
        Matrix();
        ~Matrix();

        // Create a matrix, if the matrix is already created it will be destroyed
        //
        void create(const size_t numRows, const size_t numCols);

        // destroy the actual matrix
        //
        void destroy(void);

        // Remove all the objects from the cells
        //
        void removeAllObjects(void);

        // Get the TwoDimCell associated to a row and column.
        //
        inline TwoDimCell &getCell(const size_t col, const size_t row);
        inline TwoDimCell &getCell(const size_t index);
        inline void getCell(const size_t bcol, const size_t brow,
                            const size_t ecol, const size_t erow,
                            std::vector<TwoDimCell*> &result);

        // helper function to get the index from a column and row number
        //
        inline size_t getIndex(const size_t col, const size_t row) const;

    private:
        Matrix(const Matrix&);
        Matrix &operator=(const Matrix&);

    private:
        CellsVec mCells;
        size_t mRows;
        size_t mCols;

    };

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
    void addObject(Object *obj);

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

    /**
     * @brief Check if an object exists or not in the SpacePartition
     * @param obj   The object we want to check if exists or not
     */
    inline bool exists(const Object *obj) const;


    // Specific object handling functions
    //

    /**
     * @brief Translate a object from the actual position using the vector t
     * @param obj   The object to be translated (it should be in the Space
     *              Partition)
     * @param t     The translation vector
     */
    inline void translateObject(Object *obj, const math::Vector2f &t);

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
    // Auxiliary functions
    //

    /**
     * Returns the corresponding values from a vector to the matrix
     */
    inline size_t getXPosition(const float x) const;
    inline size_t getYPosition(const float y) const;

    /**
     * Get the cells associated to a Object and put the result in buff
     */
    inline void getCellsFromObject(const Object *obj,
                                   std::vector<TwoDimCell*> &buff);
    inline void getCellsFromAABB(const math::AABBf &aabb,
                                 std::vector<TwoDimCell*> &buff);


private:
    float mFactorX;
    float mFactorY;
    float mCellSizeX;
    float mCellSizeY;
    float mXBounds;
    float mYBounds;
    size_t mNumCellX;
    size_t mNumCellY;
    Matrix mMatrix;
    std::vector<TwoDimCell*> mCellAuxBuffer;
    std::vector<Object *> mObjects;
    unsigned short mRunNumber;
};



// Inline Implementations
//

/**
 * Returns the corresponding values from a vector to the matrix
 */
inline size_t
SpacePartition::getXPosition(const float x) const
{
    ASSERT(x >= 0.f);
    const size_t r = static_cast<size_t>(x * mFactorX);
    ASSERT(r < mNumCellX);
    return (r >= mNumCellX) ? mNumCellX - 1 : r;
}
inline size_t
SpacePartition::getYPosition(const float y) const
{
    ASSERT(y >= 0.f);
    const size_t r = static_cast<size_t>(y * mFactorY);
    ASSERT(r < mNumCellY);
    return (r >= mNumCellY) ? mNumCellY-1 : r;
}

inline void
SpacePartition::getCellsFromObject(const Object *obj,
                                   std::vector<TwoDimCell*> &buff)
{
    ASSERT(obj);
    buff.clear();
    const math::Vector2f &tl = obj->mAABB.tl;
    const math::Vector2f &br = obj->mAABB.br;
    mMatrix.getCell(getXPosition(tl.x), getYPosition(tl.y),
                    getXPosition(br.x), getYPosition(br.y),
                    buff);
}
inline void
SpacePartition::getCellsFromAABB(const math::AABBf &aabb,
                                 std::vector<TwoDimCell*> &buff)
{
    buff.clear();
    mMatrix.getCell(getXPosition(aabb.tl.x), getYPosition(aabb.tl.y),
                    getXPosition(aabb.br.x), getYPosition(aabb.br.y),
                    buff);
}

inline bool
SpacePartition::exists(const Object *obj) const
{
    ASSERT(obj);
    return obj->mID < mObjects.size() && mObjects[obj->mID] == obj;
}


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
inline void
SpacePartition::translateObject(Object *obj, const math::Vector2f &t)
{
    math::Vector2f pos;
    obj->position(pos);
    pos += t;
    setObjectPosition(obj, pos);
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

////////////////////////////////////////////////////////////////////////////////
inline TwoDimCell &
SpacePartition::Matrix::getCell(const size_t col, const size_t row)
{
    ASSERT(mRows != std::numeric_limits<size_t>::max());
    ASSERT(mCols != std::numeric_limits<size_t>::max());
    ASSERT(mCols * row + col < mCells.size());
    return mCells[mCols * row + col];
}
inline TwoDimCell &
SpacePartition::Matrix::getCell(const size_t index)
{
    ASSERT(index < mCells.size());
    return mCells[index];
}

inline void
SpacePartition::Matrix::getCell(const size_t bcol, const size_t brow,
                                const size_t ecol, const size_t erow,
                                std::vector<TwoDimCell*> &result)
{
    ASSERT(mRows != std::numeric_limits<size_t>::max());
    ASSERT(mCols != std::numeric_limits<size_t>::max());

    size_t bIndex = mCols * brow + bcol;
    const size_t eIndex = mCols * erow + ecol;

    ASSERT(bIndex <= eIndex);
    ASSERT(bIndex < mCells.size());
    ASSERT(eIndex < mCells.size());

    // we do not reserve the size for the vector becase we assume that we
    // we will use always the same vector
    for(; bIndex < eIndex; ++bIndex)
        result.push_back(&mCells[bIndex]);
}

inline size_t
SpacePartition::Matrix::getIndex(const size_t col, const size_t row) const
{
    return mCols * row + col;
}

}

#endif /* SPACEPARTITION_H_ */

