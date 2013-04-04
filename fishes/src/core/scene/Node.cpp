/*
 * Node.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: agustin
 */

#include "Node.h"

#include <set>
#include <deque>
#include <cmath>

#include <debug/DebugUtil.h>


namespace scene {

NodeVec *Node::sDirtyNodesCont = 0;
s_p::SpacePartition *Node::sSpaceManager = 0;

////////////////////////////////////////////////////////////////////////////////
void
Node::setDirtyNodesContainer(NodeVec *dirtyNodes)
{
    sDirtyNodesCont = dirtyNodes;
}

////////////////////////////////////////////////////////////////////////////////
void
Node::setSpacePartitionManager(s_p::SpacePartition *spaceManager)
{
    sSpaceManager = spaceManager;
}

////////////////////////////////////////////////////////////////////////////////
bool
Node::checkCycles(Node *node) const
{
    // TODO: improve this
    debugOPTIMIZATION("Improve this algorithm\n");

    std::set<const Node *> markedChilds;
    std::deque<const Node *> nodes;
    nodes.push_back(this);
    markedChilds.insert(this);
    while(!nodes.empty()){
        const Node *actualNode = nodes.front();
        nodes.pop_front();
        if (actualNode == node) {
            // cycle found
            return true;
        }
        // get the childs and put them in the queue
        const NodeVec &childrens = actualNode->getChildrens();
        for(size_t i = 0, size = childrens.size(); i < size; ++i){
            const Node *n = childrens[i];
            if (node == n){
                // cycle found
                return true;
            }
            if (markedChilds.find(n) != markedChilds.end()) {
                // cycle found!
                return true;
            }
            // add the new node to the queue
            nodes.push_back(n);
            markedChilds.insert(n);
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Node::Node()
{
    ASSERT(sDirtyNodesCont != 0);
    ASSERT(sSpaceManager != 0);
}

////////////////////////////////////////////////////////////////////////////////
Node::~Node()
{
    // TODO remove from the parent, remove the childs?
    detachFromParent();
    sSpaceManager->removeObject(&mSpaceObject);

}

////////////////////////////////////////////////////////////////////////////////
void
Node::addChild(Node *child)
{
    ASSERT(child);
    if (checkCycles(child)) {
        debugERROR("Cycle found!\n");
        ASSERT(false);
    }
    // detach from parent this one
    child->detachFromParent();

    // add the node to be child of this one
    mChilds.push_back(child);
    child->mParent = this;
}

////////////////////////////////////////////////////////////////////////////////
Node *
Node::createChild(void)
{
    Node *result = new Node();
    addChild(result);
    return result;
}

////////////////////////////////////////////////////////////////////////////////
bool
Node::isChild(const Node *child) const
{
    for(size_t i = 0, size = mChilds.size(); i < size; ++i) {
        if (mChilds[i] == child){
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void
Node::removeChild(Node *child)
{
    ASSERT(child);
    ASSERT(isChild(child));
    // remove it from the vector
    for(size_t i = 0, size = mChilds.size(); i < size; ++i){
        if (mChilds[i] == child){
            mChilds[i] = mChilds.back();
            mChilds.pop_back();
            break;
        }
    }
    child->mParent = 0;
}

////////////////////////////////////////////////////////////////////////////////
void
Node::detachFromParent(void)
{
    if (mParent == 0){
        // nothing to do
        return;
    }

    // call the parent to remove the child
    mParent->removeChild(this);

    // mParent will be set to 0 in the father
}

////////////////////////////////////////////////////////////////////////////////
void
Node::setEntity(Entity *entity)
{
    ASSERT(entity);
    mEntity = entity;
}


////////////////////////////////////////////////////////////////////////////////
void
Node::updateNodeTransformation(const math::Matrix4 &transform)
{
    ASSERT(mFlags.visible);

    if (mFlags.dirty) {
        // calculate the new matrix
        // 3.141592654f / 180.f = 0.017453293
        const float angle  = -mRotation * 0.017453293f;
        const float cosine = static_cast<float>(std::cos(angle));
        const float sine   = static_cast<float>(std::sin(angle));
        const float sxc    = mScale.x * cosine;
        const float syc    = mScale.y * cosine;
        const float sxs    = mScale.x * sine;
        const float sys    = mScale.y * sine;

        mTransformationMat.setValues(sxc, sys, mPosition.x,
                                     -sxs, syc, mPosition.y,
                                     0.f, 0.f, 1.f);
        mFlags.dirty = 0;
    }

    // create the matrix that we will use for the world transformation
    mWorldMat = transform;
    mWorldMat *= mTransformationMat;

    // compute the rect to be used for the space partition if we have an entity
    if (mEntity != 0){
        math::AABBf newAABB = mSpaceObject.aabb();
        mEntity->boundingBox(newAABB);

        // transform the aabb using the world matrix
        mWorldMat.transformAABB(newAABB);

        // now we have to update the spaceObject to the correct place
        sSpaceManager->updateObject(&mSpaceObject, newAABB);
    }
}

void
Node::updateNodeAndChildsTransforms(const math::Matrix4 &parentTransform)
{
    updateNodeTransformation(parentTransform);
    // call the childrens transformation
    for (size_t i = 0, size = mChilds.size(); i < size; ++i) {
        (mChilds[i])->updateNodeAndChildsTransforms(mWorldMat);
    }
}


}
