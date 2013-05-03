/*
 * UIManager.h
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include <vector>

#include <SFML/System/Vector2.hpp>

#include <common/debug/DebugUtil.h>
#include <core/system/GlobalConfig.h>
#include <core/math/AABB.h>

#include "EventInfo.h"

// Forward
//
namespace utils {
class MouseCursor;
}

namespace ui {
class Element;
}

namespace sf {
class RenderWindow;
}

namespace ui {

class UIManager {

    static const unsigned int NUM_ROWS = 5;
    static const unsigned int NUM_COLUMNS = 5;

    static const unsigned int MATRIX_SIZE = NUM_ROWS * NUM_COLUMNS;

    typedef std::vector<Element *> ElementVec;
    typedef std::vector<ElementVec *> VecOfElementVec;

public:
    inline UIManager(sys::GlobalConfig& gc) :
        mMouseCursor(gc.mouseCursor())
    ,   mCellSizeX(gc.windowWidth() / NUM_COLUMNS)
    ,   mCellSizeY(gc.windowHeight() / NUM_ROWS)
    ,   mRenderWindow(gc.renderWindow())
    ,   mWindowRect(math::Vector2ui(0,0),
                    math::Vector2ui(gc.windowWidth(), gc.windowHeight()))
    {
            ASSERT(mCellSizeX > 0);
            ASSERT(mCellSizeY > 0);
    }

    /**
     * @brief Add/Remove/Check a new Element to be handled
     * @note We only can add an element if it is completely inside of the region
     *       we established when build the Manager.
     */
    void
    addMenu(Element *menu);
    void
    removeMenu(Element *menu);
    bool
    hasMenu(const Element *menu);

    /**
     * @brief Update the menu and draw all the UI elements
     */
    void
    update(void);

private:

    /**
     * @brief Get the associated index given a row and column indices
     */
    inline size_t
    getIndex(size_t column, size_t row) const;

    /**
     * @brief Get the corresponding x and y positions in the matrix
     */
    inline size_t
    getXPosition(unsigned int x) const;
    inline size_t
    getYPosition(unsigned int y) const;

    /**
     * @brief Get the ElementVec's associated to an AABB (the cells of the matrix
     *        that intersect with the AABB)
     */
    inline void
    getVecsFromAABB(const math::AABBui& aabb,
                    std::vector<ElementVec*>& result);

    /**
     * @brief Render all the elements of this manager
     */
    void
    renderAll(void) const;

private:
    utils::MouseCursor &mMouseCursor;
    ElementVec mMatrix[NUM_ROWS * NUM_COLUMNS];
    unsigned int mCellSizeX;
    unsigned int mCellSizeY;
    VecOfElementVec mResult;
    math::Vector2ui mLastMousePos;
    ElementVec mLastInside;
    EventInfo mEventInfo;
    sf::RenderWindow& mRenderWindow;
    ElementVec mElements;
    const math::AABBui mWindowRect;
};




// Inline imple
//

inline size_t
UIManager::getIndex(size_t column, size_t row) const
{
    return NUM_COLUMNS * row + column;
}

inline size_t
UIManager::getXPosition(unsigned int x) const
{
    ASSERT(x >= 0);
    const size_t r = x / mCellSizeX;
    return (r >= NUM_COLUMNS) ? NUM_COLUMNS - 1 : r;
}

inline size_t
UIManager::getYPosition(unsigned int y) const
{
    ASSERT(y >= 0);

    size_t r = y / mCellSizeY;
    return (r >= NUM_ROWS) ? NUM_ROWS - 1 : r;
}

inline void
UIManager::getVecsFromAABB(const math::AABBui& aabb,
                           std::vector<ElementVec *>& result)
{
    const size_t tlx = getXPosition(aabb.tl.x),tly = getYPosition(aabb.tl.y),
                 brx = getXPosition(aabb.br.x), bry = getYPosition(aabb.br.y);
    result.clear();
    for (size_t i = tlx; i <= brx; ++i){
        for (size_t j = tly; j <= bry; ++j){
            result.push_back(&(mMatrix[getIndex(i, j)]));
        }
    }
}

}

#endif /* UIMANAGER_H_ */
