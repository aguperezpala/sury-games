/*
 * SpacePartition.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: agustin
 */

#include "SpacePartition.h"

#include <algorithm>



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
,       mNumCellX(0)
,       mNumCellY(0)
,       mRunNumber(0)
{

}

SpacePartition::~SpacePartition()
{

}

////////////////////////////////////////////////////////////////////////////////
bool
SpacePartition::build(const math::AABB<UnitType> &world, UnitType cnX, UnitType cnY)
{
    // set the world size
    mWorld = world;

    // num of cells
    mNumCellX = cnX;
    mNumCellY = cnY;

    // create the factors
    mFactorX = static_cast<UnitType>(cnX) / world.getWidth();
    mFactorY = static_cast<UnitType>(cnY) / world.getHeight();

    mMatrix.create(cnX, cnY);

    return true;
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
    getCellsFromObject(obj, mCellAuxBuffer);
    for(size_t i = 0, size = mCellAuxBuffer.size(); i < size; ++i){
        (mCellAuxBuffer[i])->addObject(obj);
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
    getCellsFromObject(obj, mCellAuxBuffer);
    for(size_t i = 0, size = mCellAuxBuffer.size(); i < size; ++i){
        (mCellAuxBuffer[i])->removeObject(obj);
    }
}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::removeAllObjects(void)
{
    mMatrix.removeAllObjects();
    mObjects.clear();
}



// Specific object handling functions
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::setObjectPosition(Object *obj, const math::Vector2<UnitType> &pos)
{
    ASSERT(obj);
    ASSERT(exists(obj));

    // First get the actual indices for the object
    const math::Vector2<UnitType> &tl = obj->mAABB.tl;
    const math::Vector2<UnitType> &br = obj->mAABB.br;
    bool dummy;
    size_t beforeBIndex = mMatrix.getIndex(getXPosition(tl.x, dummy),
                                           getYPosition(tl.y, dummy));
    const size_t beforeEIndex = mMatrix.getIndex(getXPosition(br.x, dummy),
                                                 getYPosition(br.y, dummy));

    obj->setPosition(pos);

    // get the new positions now
    size_t afterBIndex = mMatrix.getIndex(getXPosition(tl.x,dummy),
                                          getYPosition(tl.y, dummy));
    const size_t afterEIndex = mMatrix.getIndex(getXPosition(br.x, dummy),
                                                getYPosition(br.y, dummy));

    updateObject(beforeBIndex, beforeEIndex, afterBIndex, afterEIndex, obj);

}

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::updateObject(Object *obj, const math::AABB<UnitType> &aabb)
{
    ASSERT(obj);
    ASSERT(exists(obj));

    // First get the actual indices for the object
    const math::Vector2<UnitType> &tl = obj->mAABB.tl;
    const math::Vector2<UnitType> &br = obj->mAABB.br;
    bool dummy;
    size_t beforeBIndex = mMatrix.getIndex(getXPosition(tl.x, dummy),
                                           getYPosition(tl.y, dummy));
    const size_t beforeEIndex = mMatrix.getIndex(getXPosition(br.x, dummy),
                                                 getYPosition(br.y, dummy));


    obj->setAABB(aabb);

    // get the new positions now
    size_t afterBIndex = mMatrix.getIndex(getXPosition(tl.x,dummy),
                                          getYPosition(tl.y, dummy));
    const size_t afterEIndex = mMatrix.getIndex(getXPosition(br.x, dummy),
                                                getYPosition(br.y, dummy));

    updateObject(beforeBIndex, beforeEIndex, afterBIndex, afterEIndex, obj);
}


// Space Partition querys
//

////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getIntersections(const Object *obj, ConstObjectVec &objs)
{
    ASSERT(obj);
    // new run number
    ++mRunNumber;
    objs.clear();

    getCellsFromObject(obj, mCellAuxBuffer);
    for(size_t i = 0, size = mCellAuxBuffer.size(); i < size; ++i){
        (mCellAuxBuffer[i])->getCollisions(obj, objs, mRunNumber);
    }
}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::AABB<UnitType> &aabb,
                                std::uint32_t mask,
                                ConstObjectVec &result)
{
    // new run number
    ++mRunNumber;
    result.clear();

    getCellsFromAABB(aabb, mCellAuxBuffer);
    for(size_t i = 0, size = mCellAuxBuffer.size(); i < size; ++i){
        mCellAuxBuffer[i]->getCollisionQuery(aabb, result, mRunNumber, mask);
    }
}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::Vector2<UnitType> &point,
                                ConstObjectVec &result,
                                std::uint32_t mask)
{
    // new run number
    ++mRunNumber;
    result.clear();

    bool pointInside, aux;
    const size_t px = getXPosition(point.x, pointInside);
    const size_t py = getXPosition(point.y, aux);

    if (!pointInside && !aux) {
        // nothing to do
        return;
    }

    TwoDimCell &cell = mMatrix.getCell(px, py);
    cell.getCollisionQuery(point, result, mRunNumber, mask);
}


////////////////////////////////////////////////////////////////////////////////
void
SpacePartition::getObjectsQuery(const math::Vector2<UnitType> &p1,
                                const math::Vector2<UnitType> &p2,
                                ConstObjectVec &result,
                                std::uint32_t mask)
{
    ASSERT(false);
    // TODO: here we have to get first the cells that collide with the line
    // and then iterate over all checking for the objects
}


}
