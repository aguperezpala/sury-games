/*
 * MouseCursor.h
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#ifndef MOUSECURSOR_H_
#define MOUSECURSOR_H_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <common/Forwards.h>
#include <core/math/Vector2.h>
#include <core/system/GlobalConfig.h>

#include "TextureAtlas.h"


namespace utils {

class MouseCursor
{
public:
    MouseCursor(const sys::GlobalConfig& config);
    ~MouseCursor(){};

    /**
     * @brief Get the position of the mouse cursor in the screen
     */
    inline unsigned int
    getXRelative(void) const;
    inline unsigned int
    getYRelative(void) const;
    inline const math::Vector2ui&
    getPosition(void) const;

    /**
     * @brief Update the position of the mouse (always relative to the window)
     */
    inline void
    setMousePos(const math::Vector2ui& pos);
    inline void
    setMousePos(const sf::Vector2i& pos);

    /**
     * @brief Set the texture and the Texture atlas to be used for the cursors
     */
    inline void
    setTexture(const TexturePtr& texture);
    inline void
    setTextureAtlas(const TextureAtlas& atlas);

    /**
     * @brief Get/Set the current cursor icon by ID (using the TextureAtlas
     *        associated
     */
    inline unsigned int
    mouseCursor(void) const;
    inline void
    setMouseCursor(unsigned int cursor);

    /**
     * @brief Helper cast to be drawable easily
     */
    inline operator const sf::Sprite&() const;

private:
    unsigned int mWindowHeight;
    unsigned int mWindowWidth;
    unsigned int mCurrentCursor;
    math::Vector2ui mRelPosition;
    TexturePtr mTexture;
    TextureAtlas mTextureAtlas;
    sf::Sprite mSprite;
    sf::IntRect mRect;
};




// Inline impl
//
inline MouseCursor::MouseCursor(const sys::GlobalConfig& config) :
    mWindowHeight(config.windowHeight())
,   mWindowWidth(config.windowWidth())
{
}

inline unsigned int
MouseCursor::getXRelative(void) const
{
    return mRelPosition.x;
}
inline unsigned int
MouseCursor::getYRelative(void) const
{
    return mRelPosition.y;
}
inline const math::Vector2ui&
MouseCursor::getPosition(void) const
{
    return mRelPosition;
}


inline void
MouseCursor::setMousePos(const math::Vector2ui& pos)
{
    mRelPosition = pos;
    mSprite.setPosition(pos.x, pos.y);
}
inline void
MouseCursor::setMousePos(const sf::Vector2i& pos)
{
    mRelPosition.x = pos.x;
    mRelPosition.y = pos.y;
    mSprite.setPosition(pos.x, pos.y);
}

inline void
MouseCursor::setTexture(const TexturePtr& texture)
{
    mTexture = texture;
    mSprite.setTexture(*texture);
}
inline void
MouseCursor::setTextureAtlas(const TextureAtlas& atlas)
{
    mTextureAtlas = atlas;
}

/**
 * @brief Get/Set the current cursor icon by ID (using the TextureAtlas
 *        associated
 */
inline unsigned int
MouseCursor::mouseCursor(void) const
{
    return mCurrentCursor;
}
inline void
MouseCursor::setMouseCursor(unsigned int cursor)
{
    if (mCurrentCursor == cursor) {
        return;
    }
    // change the current cursor
    mTextureAtlas.getRect(cursor, mRect);
    mSprite.setTextureRect(mRect);
}

inline MouseCursor::operator const sf::Sprite&() const
{
    return mSprite;
}

} /* namespace utils */
#endif /* MOUSECURSOR_H_ */
