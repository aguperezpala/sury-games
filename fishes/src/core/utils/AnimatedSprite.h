/*
 * AnimatedSprite.h
 *
 *  Created on: Apr 27, 2013
 *      Author: agustin
 */

#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include <core/scene/Entity.h>
#include <common/Forwards.h>

#include "TextureAtlas.h"

namespace utils {

class AnimatedSprite
{
    // internal flags
    enum Flag {
        NONE =          0,
        STOPPED =       (1 << 0),
        LOOP =          (1 << 1),
        PLAYING =       (1 << 2),
    };
public:
    struct AnimIndices {
        unsigned int begin;
        unsigned int end;
        float animTime;
    };

public:
    AnimatedSprite();
    ~AnimatedSprite();

    /**
     * @brief Set the texture we want to use
     */
    inline void
    setTexture(const TexturePtr &texture);

    /**
     * @brief Configure the Animated sprite with the texture atlas we want to
     *        use
     * @param ta    The texture atlas we want to use
     */
    inline void
    setTextureAtlas(const TextureAtlas &ta);

    /**
     * @brief Create animation table. This animation table is used associate
     * sprite ranges to a given animation name (ID = size_t).
     * The frames will be:
     * 0    1   2   3
     * 4    5   6   7
     * 8    9   10  11...
     * @param   animations  The vector of ranges, where animations[i] is the i-th
     *                      animation (i its the anim ID).
     * @returns true on success or false on error.
     */
    bool
    createAnimTable(const std::vector<AnimIndices> &animations);

    /**
     * @brief Configure the animation to be played
     * @param   animID      The animation ID.
     * @param   time        If we want to change the time of the animation,
     *                      if time = -1 then we will use the time set when
     *                      create the anim table
     */
    void
    setAnim(const std::size_t animID, const float time = -1.f);

    /**
     * @brief Play / stop functions
     */
    inline void
    play(void);
    inline void
    stop(void);

    /**
     * @brief Check if the animation is stopped or not
     * @returns true if the animation is stopped, false otherwise
     */
    inline bool
    isStopped(void) const;

    /**
     * @brief Set/unset loop animation
     * @param loop  Set the animations to loop or not
     */
    inline void
    setLoop(bool loop);

    /**
     * @brief This function must be called every frame to update the animation logic
     * It will also draw the sprite into the associated window
     * @param timeFrame     The last time frame.
     */
    void
    update(float timeFrame);

    /**
     * @brief Cast to entity automatically for convenience
     */
    inline operator const scene::Entity&(void) const;

    /**
     * @brief Returns the associated sprite
     */
    inline const sf::RectangleShape&
    sprite(void) const;
    inline sf::RectangleShape&
    sprite(void);

private:

    // @brief Flags manipulation functions
    inline void
    setFlag(Flag f);
    inline void
    unsetFlag(Flag f);
    inline bool
    checkFlag(Flag f) const;
    inline void
    clearFlags(void);

    // @brief Check if the animation table is valid
    bool
    checkAnimTable(const std::vector<AnimIndices> &animation) const;

    // @brief Configure the rectangle for a given sprite index
    inline void
    configureRect(const unsigned int index);

private:
    typedef std::vector<AnimIndices> AnimationVec;


    TexturePtr mTexture;
    int mFlags;
    float mAccumTime;
    float mAnimTime;
    float mTimeFactor;
    unsigned int mFrameIndex;
    sf::IntRect mRect;
    scene::Entity mEntity;
    TextureAtlas mTextureAtlas;
    AnimationVec mAnimations;
    unsigned int mAnimIndex;
};




// Inline impl
//

inline void
AnimatedSprite::setFlag(Flag f)
{
    mFlags |= f;
}
inline void
AnimatedSprite::unsetFlag(Flag f)
{
    mFlags &= ~f;
}
inline bool
AnimatedSprite::checkFlag(Flag f) const
{
    return mFlags & f;
}
inline void
AnimatedSprite::clearFlags(void)
{
    mFlags = Flag::NONE;
}


inline void
AnimatedSprite::play(void)
{
    unsetFlag(Flag::STOPPED);
    setFlag(Flag::PLAYING);
}
inline void
AnimatedSprite::stop(void)
{
    setFlag(Flag::STOPPED);
}
inline bool
AnimatedSprite::isStopped(void) const
{
    return checkFlag(Flag::STOPPED);
}

inline void
AnimatedSprite::setLoop(bool loop)
{
    if (loop){
        setFlag(Flag::LOOP);
    } else {
        unsetFlag(Flag::LOOP);
    }
}

inline void
AnimatedSprite::setTexture(const TexturePtr &texture)
{
    ASSERT(texture.get());
    mTexture = texture;
    mEntity.sprite().setTexture(texture.get());
}

inline void
AnimatedSprite::setTextureAtlas(const TextureAtlas &ta)
{
    mTextureAtlas = ta;
}

inline AnimatedSprite::operator const scene::Entity&(void) const
{
    return mEntity;
}

inline const sf::RectangleShape&
AnimatedSprite::sprite(void) const
{
    return mEntity.sprite();
}
inline sf::RectangleShape&
AnimatedSprite::sprite(void)
{
    return mEntity.sprite();
}

inline void
AnimatedSprite::configureRect(const unsigned int index)
{
    mTextureAtlas.getRect(index, mRect);
    mEntity.sprite().setTextureRect(mRect);
}

} /* namespace utils */
#endif /* ANIMATEDSPRITE_H_ */
