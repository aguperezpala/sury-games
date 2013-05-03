/*
 * Button.cpp
 *
 *  Created on: Apr 30, 2013
 *      Author: agustin
 */

#include "Button.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <core/math/AABB.h>
#include <core/math/Vector2.h>


namespace ui {

////////////////////////////////////////////////////////////////////////////////
void
Button::configureTextPosition(void)
{
    ASSERT(false);
}

////////////////////////////////////////////////////////////////////////////////
void
Button::elementChanged(void)
{
    if (!mTexture.get()) {
        // do nothing
        return;
    }

    // we need to update the position and the size of the sprite
    const math::AABBui& theAABB = aabb();
    const math::Vector2ui& pos = position();
    const unsigned int width = theAABB.getWidth(),
                       height = theAABB.getHeight();

    mSprite.setPosition(pos.x, pos.y);

    // calculate the size
    mSprite.setSize(sf::Vector2f(width, height));
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Button::Button(UIManager *manager) :
    Element(manager)
{
}

////////////////////////////////////////////////////////////////////////////////
Button::~Button()
{
}

////////////////////////////////////////////////////////////////////////////////
void
Button::newEvent(const EventInfo& event)
{
    ASSERT(mActivated);

    // check for state
    switch (event.event) {
    case EventInfo::Event::MouseInside:
    case EventInfo::Event::MouseMoving:
        // set the corresponding state if we should
        if (mState == State::NORMAL) {
            setState(State::MOUSE_OVER);
        }
        break;

    case EventInfo::Event::MouseOutside:
        // we always want to set back to normal state?
        setState(State::NORMAL);
        break;

    case EventInfo::Event::MouseButton:
        // check if we have to send the event to the callbacks
        if (event.ioInfo.checkButtonReleased(EventInfo::IOInfo::Mouse::LeftButton)) {
            // we should trigger the event
            mCallbacks(this, Event::LEFT_PRESSED);
            setState(State::MOUSE_OVER);
        } else  if (event.ioInfo.checkButtonPressed(
                    EventInfo::IOInfo::Mouse::LeftButton)) {
            // check if we have to change the state of the mouse (when is pressed
            setState(State::SELECTED);
        }
        break;
    default:
        debugERROR("Event: %d not handled\n", event.event);
        ASSERT(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void
Button::render(sf::RenderWindow& renderWindow) const
{
    renderWindow.draw(mSprite);
}

////////////////////////////////////////////////////////////////////////////////
void
Button::configureButton(const TexturePtr& texture, const sf::IntRect& rect)
{
    ASSERT(texture.get());
    mTexture = texture;
    mSprite.setTexture(mTexture.get());

    // configure the rect
    mRect.height = rect.height;
    mRect.width = rect.width / State::COUNT;
    mRect.top = mRect.left = 0;
    setState(State::NORMAL);

    elementChanged();
}

} /* namespace ui */
