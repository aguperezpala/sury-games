/*
 * Node.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: agustin
 */

#include "Node.h"

#include <set>
#include <deque>

#include <debug/DebugUtil.h>


namespace scene {

Node::NodeVec *Node::sDirtyNodesCont = 0;
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
    ASSERT(false);
    // TODO: improve this
    debugOPTIMIZATION("Improve this algorithm\n");

    std::set<Node *> markedChilds;
    std::deque<Node *> nodes;
    nodes.push_back(this);
    markedChilds.insert(this);
    while(!nodes.empty()){
        Node *actualNode = nodes.front();
        nodes.pop_front();
        if (actualNode == node) {
            // cycle found
            return true;
        }
        // get the childs and put them in the queue
        const NodeVec &childrens = actualNode->getChildrens();
        for(size_t i = 0, size = childrens.size(); i < size; ++i){
            Node *n = childrens[i];
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

}

////////////////////////////////////////////////////////////////////////////////
void
Node::addChild(Node *child)
{
    ASSERT(child);
    if (checkCycles(child)) {
        debuERROR("Cycle found!\n");
        ASSERT(false);
    }
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
void
Node::setEntity(Entity *entity)
{
    ASSERT(entity);
    mEntity = entity;
}


////////////////////////////////////////////////////////////////////////////////
void
Node::updateNodeTransformation(const math::Matrix4 &transform,
                               math::Matrix4 &worldTransform)
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
    worldTransform = transform;
    worldTransform *= mTransformationMat;

    // compute the rect to be used for the space partition if we have an entity
    if (mEntity != 0){
        math::AABBf & aabb = mSpaceObject.aabb();
        mEntity->boundingBox(aabb);

        // transform the aabb using the world matrix
        worldTransform.transformAABB(aabb);

        // now we have to move the spaceObject to the correct place
        math::Vector2f pos;
        aabb.center(pos);
        sSpaceManager->setObjectPosition(mSpaceObject, pos);
    }

}


}
