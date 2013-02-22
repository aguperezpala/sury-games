/*
 * SpacePartition.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#include "SpacePartition.h"


namespace s_p {


SpacePartition::Matrix::Matrix() :
    mRows(std::numeric_limits<size_t>::max())
,   mCols(std::numeric_limits<size_t>::max())
{
}

SpacePartition::Matrix::~Matrix()
{
    // do nothing
}



// Create a matrix, if the matrix is already created it will be destroyed
//
void
SpacePartition::Matrix::create(const size_t numRows, const size_t numCols)
{
    const size_t newSize = numRows * numCols;
    mCells.clear();
    mCells.resize(newSize);
    mRows = numRows;
    mCols = numCols;
}

// destroy the actual matrix
//
void
SpacePartition::Matrix::destroy(void)
{
    mCells.clear();
    mRows = std::numeric_limits<size_t>::max();
    mCols = std::numeric_limits<size_t>::max();
}


void
SpacePartition::Matrix::removeAllObjects(void)
{
    for(size_t i = 0, size = mCells.size(); i < size; ++i){
        mCells[i].removeAll();
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SpacePartition::SpacePartition() :
        mFactorX(0)
,       mFactorY(0)
,       mCellSizeX(0)
,       mCellSizeY(0)
,       mXBounds(0)
,       mYBounds(0)
,       mNumCellX(0)
,       mNumCellY(0)
{

}

SpacePartition::~SpacePartition()
{

}

////////////////////////////////////////////////////////////////////////////////
bool
SpacePartition::build(const float ssX, const float ssY,
                      const size_t cnX, const size_t cnY)
{
    // num of cells
    mNumCellX = cnX;
    mNumCellY = cnY;

    mCellSizeX = ssX / static_cast<float>(cnX);
    mCellSizeY = ssY / static_cast<float>(cnY);

    mXBounds = mNumCellX * mCellSizeX;
    mYBounds = mNumCellY * mCellSizeY;

    // create the factors
    mFactorX = static_cast<float>(cnX) / ssX;
    mFactorY = static_cast<float>(cnY) / ssY;
}

// Space Partition object handling functions
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::addObject(Object *obj)
{
    ASSERT(obj);
    if (!exists(obj)){
        // add the object
        obj->mID = mObjects.size();
        mObjects.push_back(obj);
    }

    // now we have to add to the respectives cells
    getCellsFromObject(obj);
    for(size_t i = 0, size = mCellAuxBuffer.size(); i < size; ++i){
        mCellAuxBuffer[i]->addObject(obj);
    }
}

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::removeObject(const Object *obj)
{
    ASSERT(obj);
    if (!exists(obj)){
        return;
    }

    // swap with the last one
    mObjects.back()->mID = obj->mID;
    mObjects[obj->mID] = mObjects.back();
    mObjects.pop_back();

    // else we have to remove the object from the cells where it is
    getCellsFromObject(obj);
    for(size_t i = 0, size = mCellAuxBuffer.size(); i < size; ++i){
        mCellAuxBuffer[i]->removeObject(obj);
    }
}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::removeAllObjects(void)
{
    mMatrix.removeAllObjects();
}



// Specific object handling functions
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::translateObject(Object *obj, const math::Vector2f &t)
{
    ASSERT(obj);
    // TODO: use a sort intersection algorithm to get the new cells
    // and let the old ones just like they are.
}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::setObjectPosition(Object *obj, const math::Vector2f &pos)
{
    ASSERT(obj);
    // TODO: use a sort intersection algorithm to get the new cells
    // and let the old ones just like they are.
}



// Space Partition querys
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getIntersections(const Object *obj, ConstObjectVec &objs)
{
    ASSERT(obj);
}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::AABBf &aabb,
                                uint32_t mask,
                                ConstObjectVec &result)
{

}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::Vector2f &point,
                                ConstObjectVec &result,
                                uint32_t mask)
{

}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::Vector2f &p1,
                                const math::Vector2f &p2,
                                ConstObjectVec &result,
                                uint32_t mask)
{

}


}
