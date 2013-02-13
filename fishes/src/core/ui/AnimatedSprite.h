/*
 * AnimatedSprite.h
 *
 *  Created on: Feb 12, 2013
 *      Author: agustin
 */

#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include <string>
#include <cstddef>
#include <boost/shared_ptr.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace ui {

class AnimatedSprite
{
    // internal flags
    enum Flag {
        NONE =          0,
        STOPPED =       (1 << 0),
        LOOP =          (1 << 1),
    };
public:
    struct AnimIndices {
        std::size_t begin;
        std::size_t end;
        float animTime;
    };
public:
    AnimatedSprite();
    ~AnimatedSprite();

    // Returns the sprite that uses
    inline const sf::Sprite &sprite(void) const;
    inline sf::Sprite &sprite(void);

    // Construct the animated sprite from file (texture) and set the size of
    // the animation (in width and height). The sprite number will be ordered
    // like this:
    // 0    1   2   3
    // 4    5   6   7
    // 8    9   10  11...
    // Where each number represent the rectangle.
    // @param   textFName   Texture file name.
    // @param   numColumns  The number of columns
    // @param   numRows     The number of rows
    bool build(const std::string &textFName,
               std::size_t numColumns = 1,
               std::size_t numRows = 1);

    // Create animation table. This animation table is used associate sprite
    // ranges to a given animation name (ID = size_t).
    // @param   animations  The vector of ranges, where animations[i] is the i-th
    //                      animation (i its the anim ID).
    // @returns true on success or false on error.
    bool createAnimTable(const std::vector<AnimIndices> &animations);

    // Configure the animation to be played
    // @param   animID      The animation ID.
    // @param   time        If we want to change the time of the animation,
    //                      if time = -1 then we will use the time set when
    //                      create the anim table
    void setAnim(const std::size_t animID, const float time = -1);

    // Play / stop functions
    inline void play(void);
    inline void stop(void);
    inline bool isStopped(void) const;

    // Set/unset loop animation
    inline void setLoop(bool loop);

    // This function must be called every frame to update the animation logic
    void update(float timeFrame);

private:
    // Flags manipulation functions
    inline void setFlag(Flag f);
    inline void unsetFlag(Flag f);
    inline bool checkFlag(Flag f) const;
    inline void clearFlags(void);

    // Check if the animation table is valid
    bool checkAnimTable(const std::vector<AnimIndices> &animation) const;

    // Configure the rectangle for a given sprite index
    void configureRect(const std::size_t index);

private:
    typedef std::vector<AnimIndices> AnimationVec;


    sf::Sprite mSprite;
    boost::shared_ptr<sf::Texture> mTexture;
    int mFlags;
    float mAccumTime;
    float mAnimTime;
    sf::IntRect mRect;
    std::size_t mNumRows;
    std::size_t mNumColumns;
    AnimationVec mAnimations;
    std::size_t mAnimIndex;
};


// Inline implementations
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


inline const sf::Sprite &
AnimatedSprite::sprite(void) const
{
    return mSprite;
}
inline sf::Sprite &
AnimatedSprite::sprite(void)
{
    return mSprite;
}

inline void
AnimatedSprite::play(void)
{
    unsetFlag(Flag::STOPPED);
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

} /* namespace ui */
#endif /* ANIMATEDSPRITE_H_ */
