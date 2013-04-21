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

class AnimatedSprite : public sf::Sprite
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
        std::size_t begin;
        std::size_t end;
        float animTime;
    };
public:
    AnimatedSprite();
    ~AnimatedSprite();

    // @brief Construct the animated sprite from file (texture) and set the size
    // of the animation (in width and height). The sprite number will be ordered
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

    // @brief Create animation table. This animation table is used associate
    // sprite ranges to a given animation name (ID = size_t).
    // The frames will be:
    // 0    1   2   3
    // 4    5   6   7
    // 8    9   10  11...
    // @param   animations  The vector of ranges, where animations[i] is the i-th
    //                      animation (i its the anim ID).
    // @returns true on success or false on error.
    bool createAnimTable(const std::vector<AnimIndices> &animations);

    // @brief Configure the animation to be played
    // @param   animID      The animation ID.
    // @param   time        If we want to change the time of the animation,
    //                      if time = -1 then we will use the time set when
    //                      create the anim table
    void setAnim(const std::size_t animID, const float time = -1.f);

    // @brief Play / stop functions
    inline void play(void);
    inline void stop(void);

    // @brief Check if the animation is stopped or not
    // @returns true if the animation is stopped, false otherwise
    inline bool isStopped(void) const;

    // @brief Set/unset loop animation
    // @param loop  Set the animations to loop or not
    inline void setLoop(bool loop);

    // @brief This function must be called every frame to update the animation logic
    // It will also draw the sprite into the associated window
    // @param timeFrame     The last time frame.
    void update(float timeFrame);


private:
    // @brief Flags manipulation functions
    inline void setFlag(Flag f);
    inline void unsetFlag(Flag f);
    inline bool checkFlag(Flag f) const;
    inline void clearFlags(void);

    // @brief Check if the animation table is valid
    bool checkAnimTable(const std::vector<AnimIndices> &animation) const;

    // @brief Configure the rectangle for a given sprite index
    void configureRect(const std::size_t index);

private:
    typedef std::vector<AnimIndices> AnimationVec;


    boost::shared_ptr<sf::Texture> mTexture;
    int mFlags;
    float mAccumTime;
    float mAnimTime;
    float mTimeFactor;
    std::size_t mFrameIndex;
    sf::IntRect mRect;
    std::size_t mNumRows;
    std::size_t mNumColumns;
    AnimationVec mAnimations;
    std::size_t mAnimIndex;
};

// Typedef this as AnimatedSpritePtr
typedef boost::shared_ptr<AnimatedSprite> AnimatedSpritePtr;

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

} /* namespace ui */
#endif /* ANIMATEDSPRITE_H_ */
