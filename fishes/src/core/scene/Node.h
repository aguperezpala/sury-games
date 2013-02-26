/*
 * Node.h
 *
 *  Created on: Feb 24, 2013
 *      Author: agustin
 */

#ifndef NODE_H_
#define NODE_H_

#include <vector>

#include <math/Vector2.h>
#include <math/Matrix4.h>
#include <space_partition/Object.h>
#include <space_partition/SpacePartition.h>

#include "Entity.h"

namespace scene {

// Fordward declaration
//
class Entity;

class Node {
public:

    typedef std::vector<Node *> NodeVec;

public:
    Node();
    ~Node();

    /**
     * @brief Returns the parent Node
     * @returns the Parent Node of this one
     */
    inline const Node *parent(void) const;
    inline Node *parent(void);

    /**
     * @brief Add a child to the node. The node shouldn't be child of other
     *        Node.
     * @param node  The node to be child of this one
     */
    void addChild(Node *child);

    /**
     * @brief create a child joint from this one
     * @returns the new allocated child node
     */
    Node *createChild(void);

    /**
     * @brief Returns all the childrens of this node
     * @returns the vector of the children nodes
     */
    inline const NodeVec &getChildrens(void) const;
    inline NodeVec &getChildrens(void);

    /**
     * @brief Returns the entity of this node
     * @returns the associated entity of this node
     */
    inline const Entity *getEntity(void) const;
    inline Entity *getEntity(void);

    /**
     * @brief Set the entity to be handled by this node
     * @param entity    The entity to be associated to this node
     */
    void setEntity(Entity *entity);

    /**
     * @brief Show / Hide the node in the scene
     * @param   show    If show is true the scene node will be shown in the screen
     *                  if show is false the scene node will not.
     */
    inline void setVisible(bool show);
    inline bool isVisible(void) const;

    // Node transformations function
    //

    /**
     * @brief Set the position of this node (relative to the parent)
     * @param pos   The new position of the node
     */
    inline void setPosition(const math::Vector2f &pos);
    inline void setPosition(float x, float y);

    /**
     * @brief Return the actual position of the node
     * @returns the position of the node
     */
    inline const math::Vector2f &position(void) const;

    /**
     * @brief Translate the actual node (in local space)
     * @param t     The translation vector
     */
    inline void translate(const math::Vector2f &t);
    inline void translate(float x, float y);

    /**
     * @brief Scale the current node (local space)
     * @param s     The scale vector to be applied
     */
    inline void setScale(const math::Vector2f &s);
    inline void setScale(float x, float y);

    /**
     * @brief Returns the actual scale
     * @returns the actual scale
     */
    inline const math::Vector2f &scale(void) const;

    /**
     * @brief Apply a rotation to the node (in local space)
     * @param angle The angle to be applied
     */
    inline void setRotation(float angle);

    /**
     * @brief Get the rotation of the node
     * @returns the rotation of the node
     */
    inline float rotation(void) const;

    /**
     * @brief Set the new local matrix transformation
     * @param m     The transformation matrix to be set
     */
    inline void setTransformMat(const math::Matrix4 &m);

    /**
     * @brief Get the transformation matrix associated to this node (local)
     * @returns the local transformation matrix
     */
    inline const math::Matrix4 &transformationMat(void) const;

    /**
     * @brief Set the vector where the dirty nodes will be pushed back
     * @param dirtyNodes    The vector where the dirty nodes will be stored
     */
    static void setDirtyNodesContainer(NodeVec *dirtyNodes);

    /**
     * @brief Set the space partition manager used by the scene
     * @param spaceManager    The SpacePartition manager we want to use
     */
    static void setSpacePartitionManager(s_p::SpacePartition *spaceManager);

private:
    // TODO: friend class here the SceneManager

    /**
     * @brief Function called by the SceneManager to update the world node
     *        transformation (using the accumulated parents matrices)
     * @param   transform       The parent accumulated transformation
     * @param   worldMatrix     The resulting world matrix for this node
     */
    void updateNodeTransformation(const math::Matrix4 &transform,
                                  math::Matrix4 &worldMatrix);

    /**
     * @brief Auxiliary function to mark the node as dirty if it wasn't and
     *        put it in the dirty nodes container
     */
    inline void handleDirty(void);

    /**
     * @brief Function that checks for cicles in the graph
     * @param node  The node that will be parent of this one
     * @returns     true if there are cycles, false otherwise
     */
    bool checkCycles(Node *node) const;

    /**
     * @brief Returns the associate space object to be handled from outside
     * @returns spaceObject associated to this node
     */
    inline s_p::Object &spaceObject(void);

private:

    struct Flags {
        unsigned char dirty : 1;
        unsigned char visible : 1;
    };

    // used to represent the AABB of this node
    s_p::Object mSpaceObject;
    // values of the node that will form the transformation matrix
    math::Vector2f mPosition;
    math::Vector2f mScale;
    float mRotation;
    math::Matrix4 mTransformationMat;
    // Nodes and entity
    Node *mParent;
    NodeVec mChilds;
    Entity *mEntity;
    // flags of the node
    Flags mFlags;
    // Id of the node
    size_t mID;

    // static container for all the nodes
    static NodeVec *sDirtyNodesCont;
    // the SpacePartition manager used for the scene
    static s_p::SpacePartition *sSpaceManager;
};


// Inline implementations
//

inline void
Node::handleDirty(void)
{
    if (mFlags.dirty) {
        return;
    }
    // if isn't dirty then mark it and put it in the container
    mFlags.dirty = 1;
    mID = sDirtyNodesCont->size();
    sDirtyNodesCont->push_back(this);
}

inline s_p::Object &
Node::spaceObject(void)
{
    return mSpaceObject;
}

inline const Node *
Node::parent(void) const
{
    return mParent;
}
inline Node *
Node::parent(void)
{
    return mParent;
}

inline const Node::NodeVec &
Node::getChildrens(void) const
{
    return mChilds;
}
inline Node::NodeVec &
Node::getChildrens(void)
{
    return mChilds;
}

inline const Entity *
Node::getEntity(void) const
{
    return mEntity;
}
inline Entity *
Node::getEntity(void)
{
    return mEntity;
}

inline void
Node::setVisible(bool show)
{
    mFlags.visible = show;
}
inline bool
Node::isVisible(void) const
{
    return mFlags.visible;
}

// Node transformations function
//
inline void
Node::setPosition(const math::Vector2f &pos)
{
    mPosition = pos;
    handleDirty();
}
inline void
Node::setPosition(float x, float y)
{
    mPosition.x = x;
    mPosition.y = y;
    handleDirty();
}


inline const math::Vector2f &
Node::position(void) const
{
    return mPosition;
}


inline void
Node::translate(const math::Vector2f &t)
{
    mPosition += t;
    handleDirty();
}
inline void
Node::translate(float x, float y)
{
    mPosition.x += x;
    mPosition.y += y;
    mFlags.positionChange = 1;
    handleDirty();
}

inline void
Node::setScale(const math::Vector2f &s)
{
    mScale = s;
    handleDirty();
}
inline void
Node::setScale(float x, float y)
{
    mScale.x = x;
    mScale.y = y;
    handleDirty();
}
inline const math::Vector2f &
Node::scale(void) const
{
    return mScale;
}

inline void
Node::setRotation(float angle)
{
    mRotation = angle;
    handleDirty();
}

inline float
Node::rotation(void) const
{
    return mRotation;
}


inline void
Node::setTransformMat(const math::Matrix4 &m)
{
    mTransformationMat = m;
    handleDirty();
}

inline const math::Matrix4 &
Node::transformationMat(void) const
{
    return mTransformationMat;
}


}

#endif /* NODE_H_ */
