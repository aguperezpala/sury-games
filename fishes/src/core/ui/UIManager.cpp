/*
 * UIManager.cpp
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#include "UIManager.h"

#include <SFML/Window/Mouse.hpp>

#include <core/utils/MouseCursor.h>

#include "Element.h"


// Auxiliary methods
//
namespace {

// Check for the existance of an element in a vector
//
template<typename T>
inline bool
elementExists(const std::vector<T> &vec, const T elem)
{
    for (size_t size = vec.size(), i = 0; i < size; ++i) {
        if (vec[i] == elem) {
            return true;
        }
    }
    return false;
}

// get an index of an element in a vector
//
template<typename T>
inline size_t
getVecIndex(const std::vector<T> &vec, const T elem)
{
    for (size_t size = vec.size(), i = 0; i < size; ++i) {
        if (vec[i] == elem) {
            return i;
        }
    }
    ASSERT(false);
}

// Swap an element with the last one (of a vector) and remove the last
//
template <typename T>
inline void
swapAndRemoveElement(std::vector<T> &vec, const size_t index)
{
    ASSERT(index < vec.size());
    vec[index] = vec[vec.size()-1];
    vec.pop_back();
}

}


namespace ui {


// Render all the elements
//
void
UIManager::renderAll(void) const
{
    for (size_t i = 0, size = mElements.size(); i < size; ++i) {
        Element* element = mElements[i];
        if (element->isActivated() && mWindowRect.collide(element->aabb())) {
            element->render(mRenderWindow);
        }
    }
}

/**
 * @brief Add/Remove/Check a new Element to be handled
 * @note We only can add an element if it is completely inside of the region
 *       we established when build the Manager.
 */
void
UIManager::addMenu(Element *menu)
{
    ASSERT(menu);
    if (hasMenu(menu)) {
        // do nothing
        return;
    }

    // get the cells where we have to put the element
    getVecsFromAABB(menu->aabb(), mResult);
    for (size_t i = 0, size = mResult.size(); i < size; ++i) {
        mResult[i]->push_back(menu);
    }

    menu->mActivated = true;
    menu->id = mElements.size();
    mElements.push_back(menu);
}

void
UIManager::removeMenu(Element *menu)
{
    ASSERT(menu);
    if (!hasMenu(menu)) {
        // do nothing
        return;
    }
    getVecsFromAABB(menu->aabb(), mResult);
    for (size_t i = 0, size = mResult.size(); i < size; ++i) {
        swapAndRemoveElement(*(mResult[i]), getVecIndex(*(mResult[i]), menu));
    }
    menu->mActivated = false;

    swapAndRemoveElement(mElements, menu->id);
}

bool
UIManager::hasMenu(const Element *menu)
{
    ASSERT(menu);

    return menu->id < mElements.size() && mElements[menu->id] == menu;
}

void
UIManager::update(void)
{
    // update the effects
    mEffectMngr.update();

    // first of all we render all the items, note that this approach will be
    // rendered before all the logic of this frame updates
    renderAll();

    // clear te event info to be sent
    mEventInfo.event = EventInfo::Event::None;

    // get the current position of the mouse
    const math::Vector2ui& currentMousePos = mMouseCursor.getPosition();

    // check for all the possible events
    mEventInfo.ioInfo.updateState(EventInfo::IOInfo::Mouse::MiddleButton,
                                  sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle));
    mEventInfo.ioInfo.updateState(EventInfo::IOInfo::Mouse::LeftButton,
                                  sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
    mEventInfo.ioInfo.updateState(EventInfo::IOInfo::Mouse::RightButton,
                                  sf::Mouse::isButtonPressed(sf::Mouse::Button::Right));

    // if the mouse move then we do all the logic
    if(mLastMousePos == currentMousePos){
        // the mouse doesn't move, then we only update all the actual menues
        // for only check the mouse state
        if (!mEventInfo.ioInfo.someMouseButtonPressed() &&
            !mEventInfo.ioInfo.someMouseButtonReleased()) {
            // we don't want to do anything? for now we are only checking mouse
            return;
        }

        // else we have to advise all the selected elements that the mouse was
        // pressed
        mEventInfo.event = EventInfo::Event::MouseButton;
        for (size_t i = 0, size = mLastInside.size(); i < size; ++i) {
            (mLastInside[i])->newEvent(mEventInfo);
        }

        // nothing else to do
        return;
    }

    // now check for all the menus that the mouse is inside of
    const size_t mouseX = getXPosition(currentMousePos.x),
                 mouseY = getYPosition(currentMousePos.y);

    ElementVec& cell = mMatrix[getIndex(mouseX, mouseY)];

    // check for menus that are older, just in case we will set the event type
    // here, since it will be the same for all of the elements
    mEventInfo.event = EventInfo::Event::MouseOutside;
    for (size_t i = 0, size = mLastInside.size(); i < size; ++i) {
        if(!(mLastInside[i])->checkPointInside(currentMousePos)){
            // is and old one, we will advise that the mouse is outside
            Element* element = mLastInside[i];

            // remove it from this vector
            swapAndRemoveElement(mLastInside, i);
            --i;
            --size;

            // advise the element that we have move the mouse outside
            element->newEvent(mEventInfo);
        }
    }

    // now we will iterate over all the elements to see which ones are the ones
    // that should be notified that the mouse is over them
    for(size_t size = cell.size(), i = 0; i < size; ++i){
        Element* element = cell[i];
        if(element->checkPointInside(currentMousePos)){
            // check if is already in the lastInside
            if (elementExists(mLastInside, element)) {
                // update the state of the current element, since the mouse
                // is moving and the element was already in the mLastInside
                mEventInfo.event = EventInfo::Event::MouseMoving;
                element->newEvent(mEventInfo);
            } else {
                // the element is not in the mLastInside, this means that is
                // a new element that we have to track. We will notify him about
                // this and put them in the mLastInside vector to be tracked in
                // the next call
                mEventInfo.event = EventInfo::Event::MouseInside;
                element->newEvent(mEventInfo);
                mLastInside.push_back(element);
            }
        }
    }

    // update the last point
    mLastMousePos = currentMousePos;
}

}
