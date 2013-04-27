/*
 * test.cpp
 *
 *  Created on: Apr 23, 2013
 *      Author: agustin
 */

#include <UnitTest++/UnitTest++.h>

#include <core/math/AABB.h>
#include <core/math/Vector2.h>

#include "../SpacePartition.h"
#include "../Object.h"
#include "../Defines.h"

static const s_p::UnitType WORLD_WIDTH = 1024;
static const s_p::UnitType WORLD_HEIGHT = 768;
static const size_t NUM_PART_X = 7;
static const size_t NUM_PART_Y = 4;

s_p::SpacePartition spacePartition;
math::AABB<s_p::UnitType> mWorld;



typedef math::AABB<s_p::UnitType> AABB;
typedef math::Vector2<s_p::UnitType> Vec2;
typedef s_p::ConstObjectVec ResultVec;


#define PRINT_BEGIN(xx) debugGREEN("Starting test %s\n", #xx);
#define PRINT_END(xx) debugGREEN("ENDING test %s\n", #xx);

////////////////////////////////////////////////////////////////////////////////
///                             TESTS                                       ////
////////////////////////////////////////////////////////////////////////////////

TEST(CheckWorldSizeAndCellsSize)
{
    PRINT_BEGIN(CheckWorldSizeAndCellsSize);

    CHECK_EQUAL(WORLD_WIDTH, spacePartition.worldXSize());
    CHECK_EQUAL(WORLD_HEIGHT, spacePartition.worldYSize());
    CHECK_EQUAL(NUM_PART_X, spacePartition.numCellsX());
    CHECK_EQUAL(NUM_PART_Y, spacePartition.numCellsY());

    PRINT_END(CheckWorldSizeAndCellsSize);
}

TEST(AddRemoveObject)
{
    PRINT_BEGIN(AddRemoveObject);

    s_p::Object obj, obj2;
    AABB aabb(0,0,10,10), aabb2(40,40,600,600);
    obj.setAABB(aabb);
    obj2.setAABB(aabb2);


    spacePartition.addObject(&obj);
    CHECK(spacePartition.exists(&obj));
    spacePartition.addObject(&obj2);
    CHECK(spacePartition.exists(&obj2));

    spacePartition.removeObject(&obj);
    CHECK(!spacePartition.exists(&obj));
    CHECK(spacePartition.exists(&obj2));
    spacePartition.addObject(&obj);

    CHECK(spacePartition.exists(&obj));
    CHECK(spacePartition.exists(&obj2));

    spacePartition.removeAllObjects();


    CHECK(!spacePartition.exists(&obj));
    CHECK(!spacePartition.exists(&obj2));

    PRINT_END(AddRemoveObject);
}


TEST(ObjectTranslation)
{
    PRINT_BEGIN(ObjectTranslation);

    spacePartition.removeAllObjects();

    s_p::Object obj;
    AABB aabb(0,0,10,10);
    obj.setAABB(aabb);

    spacePartition.addObject(&obj);
    aabb.translate(Vec2(100, 100));
    CHECK(aabb == AABB(100,100,110,110));

    spacePartition.translateObject(&obj, Vec2(100,100));
    CHECK(obj.aabb() == AABB(100,100,110,110));

    // move outside of the world
    aabb.translate(Vec2(2*WORLD_WIDTH, 2*WORLD_HEIGHT));
    spacePartition.translateObject(&obj, Vec2(2*WORLD_WIDTH, 2*WORLD_HEIGHT));
    CHECK(obj.aabb() == aabb);

    aabb.translate(Vec2(4*WORLD_WIDTH, 4*WORLD_HEIGHT));
    spacePartition.translateObject(&obj, Vec2(4*WORLD_WIDTH, 4*WORLD_HEIGHT));
    CHECK(obj.aabb() == aabb);

    PRINT_END(ObjectTranslation);
}

TEST(ObjectSetPosition)
{
    PRINT_BEGIN(ObjectSetPosition);

    spacePartition.removeAllObjects();

    s_p::Object obj;
    AABB aabb(0,0,10,10);
    obj.setAABB(aabb);

    spacePartition.addObject(&obj);
    spacePartition.setObjectPosition(&obj, Vec2(100,100));
    CHECK(obj.aabb() == AABB(100,100,110,110));

    spacePartition.setObjectPosition(&obj, Vec2(10,10));
    CHECK(obj.aabb() == AABB(10, 10, 20, 20));

    // move outside of the world
    aabb.setPosition(Vec2(2*WORLD_WIDTH, 2*WORLD_HEIGHT));
    spacePartition.setObjectPosition(&obj, Vec2(2*WORLD_WIDTH, 2*WORLD_HEIGHT));
    CHECK(obj.aabb() == aabb);

    aabb.setPosition(Vec2(4*WORLD_WIDTH, 4*WORLD_HEIGHT));
    spacePartition.setObjectPosition(&obj, Vec2(4*WORLD_WIDTH, 4*WORLD_HEIGHT));
    CHECK(obj.aabb() == aabb);

    PRINT_END(ObjectSetPosition);
}

TEST(UpdateObjectPosition)
{
    PRINT_BEGIN(UpdateObjectPosition);

    spacePartition.removeAllObjects();

    s_p::Object obj;
    AABB aabb(0,0,10,10), abb2(40,40,600,600);
    obj.setAABB(aabb);

    spacePartition.addObject(&obj);
    spacePartition.setObjectPosition(&obj, Vec2(100,100));
    CHECK(obj.aabb() == AABB(100,100,110,110));

    spacePartition.updateObject(&obj, abb2);
    CHECK(obj.aabb() == abb2);

    spacePartition.updateObject(&obj, aabb);
    CHECK(obj.aabb() == aabb);

    PRINT_END(UpdateObjectPosition);
}

TEST(CheckSimpleColision)
{
    PRINT_BEGIN(CheckSimpleColision);

    spacePartition.removeAllObjects();

    s_p::Object obj,obj2;
    AABB aabb(0,0,10,10), aabb2(40,40,600,600);
    ResultVec result;
    obj.setAABB(aabb);
    obj2.setAABB(aabb2);

    spacePartition.addObject(&obj);
    spacePartition.addObject(&obj2);

    spacePartition.getIntersections(&obj, result);
    CHECK_EQUAL(0, result.size());
    spacePartition.getIntersections(&obj2, result);
    CHECK_EQUAL(0, result.size());

    // move and intersect them
    spacePartition.updateObject(&obj, AABB(39,39,41,44));
    spacePartition.getIntersections(&obj, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj2);

    spacePartition.getIntersections(&obj2, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj);

    PRINT_END(CheckSimpleColision);
}

TEST(DifferentsCollisions)
{
    PRINT_BEGIN(DifferentsCollisions);

    spacePartition.removeAllObjects();

    s_p::Object obj,obj2;

    // equal aabb's collisions
    AABB aabb(1,1,10,10), aabb2(1,1,10,10);
    ResultVec result;
    obj.setAABB(aabb);
    obj2.setAABB(aabb2);

    spacePartition.addObject(&obj);
    spacePartition.addObject(&obj2);

    spacePartition.getIntersections(&obj, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj2);

    spacePartition.getIntersections(&obj2, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj);

    // update one more bigger than the other
    spacePartition.updateObject(&obj, AABB(0,0,41,44));
    spacePartition.getIntersections(&obj, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj2);

    spacePartition.getIntersections(&obj2, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj);

    // check for collision in when the sides are shared
    spacePartition.updateObject(&obj, AABB(0,0,40,40));
    spacePartition.updateObject(&obj2, AABB(39,40,60,60));

    spacePartition.getIntersections(&obj, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj2);

    spacePartition.getIntersections(&obj2, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj);

    spacePartition.updateObject(&obj, AABB(10,10,20,20));
    spacePartition.updateObject(&obj2, AABB(0,0,10,11));

    spacePartition.getIntersections(&obj, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj2);

    spacePartition.getIntersections(&obj2, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj);

    PRINT_END(DifferentsCollisions);
}

TEST(MultiCollisionsMoving)
{
    PRINT_BEGIN(MultiCollisionsMoving);

    spacePartition.removeAllObjects();

    const size_t boxSize = 5;
    static const size_t numObjs = 30;
    AABB aabb(0, 0, boxSize, boxSize);
    const Vec2 transVec(boxSize + 1, boxSize + 1);
    s_p::Object obj[numObjs];
    ResultVec result;

    for (size_t i = 0; i < numObjs; ++i) {
        obj[i].setAABB(aabb);
        spacePartition.addObject(&(obj[i]));
    }

    // check that all the objects intersects
    for (size_t i = 0; i < numObjs; ++i) {
        spacePartition.getIntersections(&(obj[i]), result);
        CHECK_EQUAL(numObjs-1, result.size());
    }

    // check that if we move each one of them then the intersection of all of them
    // should be 0
    for (size_t i = 1; i < numObjs; ++i) {
        AABB trans = obj[i-1].aabb();
        trans.translate(transVec);
        spacePartition.updateObject(&(obj[i]), trans);
    }
    for (size_t i = 1; i < numObjs; ++i) {
        spacePartition.getIntersections(&(obj[i]), result);
        CHECK_EQUAL(0, result.size());
    }

    // move all of them again in the reverse order and we should get all of them
    // beeing intersecting between them
    for (size_t i = 1; i < numObjs; ++i) {
        AABB trans = obj[i].aabb();
        trans.translate(transVec*-i);
        spacePartition.updateObject(&(obj[i]), trans);
    }
    for (size_t i = 0; i < numObjs; ++i) {
        spacePartition.getIntersections(&(obj[i]), result);
        CHECK_EQUAL(numObjs-1, result.size());
    }

    PRINT_END(MultiCollisionsMoving);
}





// Configure the Space partition
//
static void
configureSpacePartition(void)
{
    mWorld.tl.x = 0;
    mWorld.tl.y = 0;
    mWorld.br.y = WORLD_HEIGHT;
    mWorld.br.x = WORLD_WIDTH;

    CHECK(spacePartition.build(mWorld, NUM_PART_X, NUM_PART_Y));
}


int
main(void)
{
    // configure everything
    configureSpacePartition();

    return UnitTest::RunAllTests();
}
