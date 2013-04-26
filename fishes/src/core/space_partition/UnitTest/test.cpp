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



////////////////////////////////////////////////////////////////////////////////
///                             TESTS                                       ////
////////////////////////////////////////////////////////////////////////////////

TEST(CheckWorldSizeAndCellsSize)
{
    CHECK_EQUAL(WORLD_WIDTH, spacePartition.worldXSize());
    CHECK_EQUAL(WORLD_HEIGHT, spacePartition.worldYSize());
    CHECK_EQUAL(NUM_PART_X, spacePartition.numCellsX());
    CHECK_EQUAL(NUM_PART_Y, spacePartition.numCellsY());
}

TEST(AddRemoveObject)
{
    s_p::Object obj, obj2;
    AABB aabb(0,0,10,10), aabb2(40,40,600,600);
    obj.setAABB(aabb);
    obj2.setAABB(aabb2);


    spacePartition.addObject(&obj);
    CHECK(spacePartition.exists(&obj));
    spacePartition.addObject(&obj2);
    CHECK(spacePartition.exists(&obj2));

    spacePartition.addObject(&obj);
    spacePartition.removeObject(&obj);
    CHECK(!spacePartition.exists(&obj));
    CHECK(spacePartition.exists(&obj2));
    spacePartition.addObject(&obj);

    CHECK(spacePartition.exists(&obj));
    CHECK(spacePartition.exists(&obj2));

    spacePartition.removeAllObjects();


    CHECK(!spacePartition.exists(&obj));
    CHECK(!spacePartition.exists(&obj2));
}


TEST(ObjectTranslation)
{
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
}

TEST(ObjectSetPosition)
{
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
}

TEST(UpdateObjectPosition)
{
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
}

TEST(CheckSimpleColision)
{
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
}

TEST(DifferentsCollisions)
{
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
    spacePartition.updateObject(&obj2, AABB(0,0,5,10));

    spacePartition.getIntersections(&obj, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj2);

    spacePartition.getIntersections(&obj2, result);
    CHECK_EQUAL(1, result.size());
    CHECK(result[0] == &obj);

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
