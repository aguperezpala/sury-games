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

#include "Entity.h"

namespace scene {

// Fordward declaration
//
class Entity;

class Node {
public:

    typedef std::vector<Node *> NodeVec;
    typedef std::vector<Entity *> EntityVec;

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
    void addChilde(Node *child);

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
     * @brief Returns the entities of this node
     * @returns the vector of entities associated to this node
     */
    inline const EntityVec &getEntities(void) const;
    inline EntityVec &getEntities(void);


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
     * @brief Returns the current rectangle of this node (bounding box)
     *        in the world position.
     * @returns the rectangle in the world position
     */
    inline const math::AABBf &worldAABB(void) const;



private:

    struct Flags {
        unsigned char dirty : 1;
    };

    math::AABBf mRect;
    math::Vector2f mPosition;
    math::Vector2f mScale;
    float mRotation;
    math::Matrix4 mTransformationMat;
    Node *mParent;
    NodeVec mChilds;
    EntityVec mEntities;
    Flags mFlags;
};

}

#endif /* NODE_H_ */
