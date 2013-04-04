/*
 * SceneManager.cpp
 *
 *  Created on: Feb 26, 2013
 *      Author: agustin
 */

#include "SceneManager.h"

#include <set>



namespace scene {


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager(const size_t numRows,
                           const size_t numCols,
                           const math::AABBf &world)
{
    if (!mSpacePartition.build(world.getWidth(), world.getHeight(), numCols,
        numRows)){
        debugERROR("Error creating the world space partition\n");
        ASSERT(false);
    }

    // Configure the nodes
    Node::setSpacePartitionManager(&mSpacePartition);
    Node::setDirtyNodesContainer(&mDirtyNodes);

    // create the root node
    mRootNode = new Node();
//    mRootNode->attachSpaceObject();


}

////////////////////////////////////////////////////////////////////////////////
SceneManager::~SceneManager()
{
    delete mRootNode;
}


////////////////////////////////////////////////////////////////////////////////
void
SceneManager::update(void)
{
    // TODO: we can improve this using a boost::bit_set to see which nodes were
    // already checked.
    // We will use for now a set to get a unique set of root nodes that we
    // have to update because of they dirtyness
    std::set<Node *> dirtyRoots;
    for(size_t i = 0, size = mDirtyNodes.size(); i < size; ++i){
        dirtyRoots.insert(getHighestDirtyRoot(mDirtyNodes[i]));
    }

    // clear the nodes
    mDirtyNodes.clear();

    // update each of the nodes
    for(std::set<Node *>::iterator it = dirtyRoots.begin(),
        eIt = dirtyRoots.end(); it != eIt; ++it){
        // the root node couldn't be the rootNode
        ASSERT(*it != mRootNode);

        // we have to update down the hierarchy of the root node
        // TODO:
    }
}

} /* namespace scene */