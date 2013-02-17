/*
 * AnimatedSprite.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: agustin
 */

#include "AnimatedSprite.h"

#include <SFML/System/Vector2.hpp>

#include <debug/DebugUtil.h>


// auxiliar functions
namespace {
// Get the index from a linear function (its a map of time to index)
inline std::size_t
getIndexFromTime(const float currTime, const float factor,
                 const std::size_t begIndx, const std::size_t endIdnx)
{
    ASSERT(endIdnx >= begIndx);
    return begIndx +
            static_cast<std::size_t>((currTime * factor) * (endIdnx - begIndx + 1));
}
}

namespace ui {

////////////////////////////////////////////////////////////////////////////////
bool
AnimatedSprite::checkAnimTable(const std::vector<AnimIndices> &animation) const
{
    // this function is ultra verbose just to print almost all the possible
    // errors at once
    bool valid = true;
    const std::size_t numSprites = mNumColumns * mNumRows;
    for(std::size_t i = 0, size = animation.size(); i < size; ++i){
        const AnimIndices &animI = animation[i];
        if (animI.animTime < 0.f) {
            valid = false;
            debugERROR("Animation %zu has invalid time\n", i);
        }
        if (animI.begin > numSprites) {
            valid = false;
            debugERROR("Animation %zu has invalid begin: %zu\n", i, animI.begin);
        }
        if (animI.end > numSprites) {
            valid = false;
            debugERROR("Animation %zu has invalid end: %zu\n", i, animI.end);
        }
        if (animI.begin > animI.end) {
            valid = false;
            debugERROR("Animation %zu has invalid begin[%zu] or end[%zu]\n",
                i, animI.begin, animI.end);
        }
    }
    return valid;
}

////////////////////////////////////////////////////////////////////////////////
void
AnimatedSprite::configureRect(const std::size_t index)
{
    const std::size_t row = index / mNumColumns;
    const std::size_t col = index - (mNumColumns * row);
    ASSERT(row < mNumRows);
    ASSERT(col < mNumColumns);

    // put the rectangle over there
    mRect.top = row * mRect.height;
    mRect.left = col * mRect.width;

    setTextureRect(mRect);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
AnimatedSprite::AnimatedSprite() :
    mFlags(Flag::NONE)
,   mAccumTime(0.f)
,   mAnimTime(0.f)
,   mTimeFactor(0.f)
,   mNumRows(0u)
,   mNumColumns(0u)
,   mAnimIndex(0u)
{

}

////////////////////////////////////////////////////////////////////////////////
AnimatedSprite::~AnimatedSprite()
{

}


////////////////////////////////////////////////////////////////////////////////
bool
AnimatedSprite::build(const std::string &textFName,
                      std::size_t numColumns,
                      std::size_t numRows)
{
    if (textFName.empty()) {
        debugERROR("textFName is empty\n");
        return false;
    }
    // check if we have to create a new texture
    if (mTexture.get() == 0){
        mTexture.reset(new sf::Texture());
    }
    if (!mTexture->loadFromFile(textFName)) {
        debugERROR("Error loading texture: %s\n", textFName.c_str());
        return false;
    }

    // set the texture to the sprite
    setTexture(*mTexture.get());

    // texture loaded ok, configure the rectangle size now
    sf::Vector2u textSize = mTexture->getSize();
    mRect.width = textSize.x / numColumns;
    mRect.height = textSize.y / numRows;

    mNumColumns = numColumns;
    mNumRows = numRows;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
AnimatedSprite::createAnimTable(const std::vector<AnimIndices> &animations)
{
    // only check this in debug
    ASSERT(checkAnimTable(animations));

    // copy the table
    mAnimations = animations;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void
AnimatedSprite::setAnim(const std::size_t animID, const float time)
{
    ASSERT(animID < mAnimations.size());

    // check which is the time that we should use
    mAnimTime = (time < 0.f) ? mAnimations[animID].animTime : time;
    mTimeFactor = 1.f / mAnimTime;
    mAccumTime = 0.f;

    // configure the rectangle
    mAnimIndex = animID;
    mFrameIndex = mAnimations[mAnimIndex].begin;
    configureRect(mFrameIndex);
    setFlag(Flag::PLAYING);
}

////////////////////////////////////////////////////////////////////////////////
void
AnimatedSprite::update(float timeFrame)
{
    if (checkFlag(Flag::STOPPED) || !checkFlag(Flag::PLAYING)) {
        return;
    }

    // update the accum time and get the corresponding anim index
    mAccumTime += timeFrame;
    if (mAccumTime >= mAnimTime){
        // check if is loop
        if (checkFlag(Flag::LOOP)){
            mAccumTime = 0.f;
            return;
        }
        // disable anim
        unsetFlag(Flag::PLAYING);
        return;
    }

    // check which is the frame we have to show
    const std::size_t frameIndex = getIndexFromTime(mAccumTime,
                                                    mTimeFactor,
                                                    mAnimations[mAnimIndex].begin,
                                                    mAnimations[mAnimIndex].end);
    if (frameIndex != mFrameIndex){
        // configure the new frame
        mFrameIndex = frameIndex;
        configureRect(frameIndex);
    }

}


} /* namespace ui */
