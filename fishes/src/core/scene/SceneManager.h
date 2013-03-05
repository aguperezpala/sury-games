/*
 * SceneManager.h
 *
 *  Created on: Feb 26, 2013
 *      Author: agustin
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <vector>
#include <iterator>

#include <space_partition/SpacePartition.h>
#include <common/debug/DebugUtil.h>
#include <math/Matrix4.h>

#include "Node.h"

namespace scene {


class SceneManager
{
public:
    /**
     * @brief Constructor, takes the number of rows and columnts that we want
     *        to divide the world.
     * @param numRows   The number of vertical divisions (rows)
     * @param numCols   The number of horizontal divisions (columns)
     * @param world     The size of the world where all the objects will be
     */
    SceneManager(const size_t numRows,
                 const size_t numCols,
                 const math::AABBf &world);

    ~SceneManager();

    /**
     * @brief Returns the root scene node
     * @returns The root scene node
     */
    inline Node &
    rootNode(void);
    inline const Node &
    rootNode(void) const;

    /**
     * @brief Update the all the Nodes of the scene and process the
     *        correspondings updates.
     */
    void update(void);

private:
    /**
     * @brief Find the last root of a node that is also dirty.
     * @param node  The node that we want to find the parent root that is dirty
     * @returns     The highest node (root) that is dirty of the given node
     */
    inline Node *
    getHighestDirtyRoot(Node *node) const;

    /**
     * @brief Update (down the hierarchy) the transformations of a specific node
     * @param node    The node we want to update the transformations (and their
     *                childs)
     */
    inline void
    updateHierarchy(Node *node);


private:
    // Root node
    Node *mRootNode;
    NodeVec mDirtyNodes;
    s_p::SpacePartition mSpacePartition;
    std::vector<Node *> mNodeQueue;
};


// Inline implementations
//
inline Node &
SceneManager::rootNode(void)
{
    return *mRootNode;
}

inline const Node &
SceneManager::rootNode(void) const
{
    return *mRootNode;
}

inline Node *
SceneManager::getHighestDirtyRoot(Node *node) const
{
    ASSERT(node);
    Node *dirtyRoot = node;
    while (node->parent()){
        node = node->parent();
        if (node->isDirty()){
            dirtyRoot = node;
        }
    }
    return dirtyRoot;
}

inline void
SceneManager::updateHierarchy(Node *node)
{
    ASSERT(node->parent());
    mNodeQueue.push_back(node);
    while (!mNodeQueue.empty()) {
        Node *backNode = mNodeQueue.back();
        mNodeQueue.pop_back();
        mNodeQueue.insert(backNode->getChildrens().begin(),
                          backNode->getChildrens().end(),
                          std::back_inserter(mNodeQueue));

        // transform the current node
        ASSERT(backNode->parent() != 0);
        backNode->updateNodeTransformation(backNode->parent()->getWorldMat());
    }

}

} /* namespace scene */
#endif /* SCENEMANAGER_H_ */
