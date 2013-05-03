/*
 * Button.h
 *
 *  Created on: Apr 30, 2013
 *      Author: agustin
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <boost/signal.hpp>
#include <boost/signals/slot.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>


#include <common/Forwards.h>
#include <common/debug/DebugUtil.h>

#include "Element.h"

namespace ui {

// Forward
//
class UIManager;


class Button : public Element
{
public:
    // Define the different states that could have the button
    //
    enum State {
        NORMAL = 0,
        MOUSE_OVER,
        SELECTED,
        COUNT
    };

    enum Event {
        LEFT_PRESSED = 0,
        RIGHT_PRESSED,      // not implemented
        MIDDLE_PRESSED      // not implemented
    };

    // define the signals types
    //
    typedef boost::signals::connection Connection;
    typedef boost::signal<void (Button *, Event)> Signal;

public:
    Button(UIManager *manager);
    virtual
    ~Button();

    /**
     * @brief This method will be called when a new event occur for a particular
     *        Element (for example: MouseOver, MouseExit, MouseClick, etc)
     * @parma event     The event info
     */
    virtual void
    newEvent(const EventInfo& event);

    /**
     * @brief Virtual method to render this element. We will pass the RenderWindow
     *        since some of this element could contain more than one sprite and
     *        that's why we need to pass the render window
     * @param renderWindow The render window where we will render this item
     */
    virtual void
    render(sf::RenderWindow& renderWindow) const;

    /**
     * @brief Add/remove callbacks here
     * @param callback  The callback to be added
     */
    inline Connection
    addButtonCallback(Signal::slot_type subscriber);
    inline void
    removeButtonCallback(Connection& subscriber);

    /**
     * @brief Configure the button to use a certain texture. We will use all the
     *        image (texture) if the rectangle is not specified. Otherwise we
     *        can set the size of the texture we want to use (the rectangle of
     *        the image we want to use for all the states of the Button). This
     *        rectangle should contain 1 row (and N columns where N is the number
     *        of states)
     * @param texture   The texture to be used
     * @param rect      The texture rectangle we will use.
     */
    inline void
    configureButton(const TexturePtr& texture);
    void
    configureButton(const TexturePtr& texture, const sf::IntRect& rect);

    /**
     * @brief Set/get the state of the mouse
     */
    inline State
    state(void) const;
    inline void
    setState(State s);

    ////////////////////////////////////////////////////////////////////////////
    //                         Text configuration                             //
    ////////////////////////////////////////////////////////////////////////////
    //

    /**
     * @brief Set the text to be used (configured with the character size, font,
     *        color, etc)
     * @param text      The text to be used
     */
    void
    setButtonText(const TextPtr& text);
    inline const TextPtr&
    getButtonText(void) const;
    inline TextPtr&
    getButtonText(void);

    /**
     * @brief This method should be called if the text has changed to be updated
     *        in the button itself
     */
    inline void
    updateButtonText(void);

protected:

    /**
     * @brief This method will be called every time the element change (position.
     *        size... basically, when the AABB change, this method is called).
     *        Should be implemented by the inherited elements if needed
     */
    virtual void
    elementChanged(void);

private:

    /**
     * @brief Method used to configure the text where corresponds
     */
    void
    configureTextPosition(void);

private:
    sf::RectangleShape mSprite;
    TexturePtr mTexture;
    State mState;
    sf::IntRect mRect;
    TextPtr mText;
    Signal mCallbacks;
};




// Inline Impl
//

inline Button::Connection
Button::addButtonCallback(Signal::slot_type subscriber)
{
    return mCallbacks.connect(subscriber);
}
inline void
Button::removeButtonCallback(Connection &subscriber)
{
    subscriber.disconnect();
}

inline void
Button::configureButton(const TexturePtr& texture)
{
    ASSERT(texture.get());

    mRect.left = mRect.top = 0;
    const sf::Vector2u texSize = texture->getSize();
    mRect.width = texSize.x;
    mRect.height = texSize.y;
    configureButton(texture, mRect);
}

inline Button::State
Button::state(void) const
{
    return mState;
}

inline void
Button::setState(State s)
{
    if (mState == s) {
        // do nothing
        return;
    }
    // else, set the state of the current button
    ASSERT(s < State::COUNT);
    mRect.left = mRect.width * s;
    mSprite.setTextureRect(mRect);
    mState = s;

    const sf::FloatRect t2 = mSprite.getGlobalBounds();
    debugRED("NEW SIZE: %f, %f\n", t2.width, t2.height);
}

inline void
Button::setButtonText(const TextPtr& text)
{
    ASSERT(text.get());
    mText = text;
    configureTextPosition();
}

inline const TextPtr&
Button::getButtonText(void) const
{
    return mText;
}

inline TextPtr&
Button::getButtonText(void)
{
    return mText;
}

inline void
Button::updateButtonText(void)
{
    configureTextPosition();
}

} /* namespace ui */
#endif /* BUTTON_H_ */
