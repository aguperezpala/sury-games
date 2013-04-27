/*
 * AnimatedSprite.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: agustin
 */

#include "AnimatedSprite.h"


// auxiliary functions
//
namespace {

// Get the index from a linear function (its a map of time to index)
inline std::size_t
getIndexFromTime(const float currTime, const float factor,
                 const unsigned int begIndx, const unsigned int endIdnx)
{
    ASSERT(endIdnx >= begIndx);
    return begIndx + static_cast<std::size_t>(
        (currTime * factor) * (endIdnx - begIndx + 1));
}

}

namespace utils {

////////////////////////////////////////////////////////////////////////////////
bool
AnimatedSprite::checkAnimTable(const std::vector<AnimIndices> &animation) const
{
    // this function is ultra verbose just to print almost all the possible
    // errors at once
    bool valid = true;
    const unsigned int numSprites = mTextureAtlas.numColumns() * mTextureAtlas.numRows();

    for (std::size_t i = 0, size = animation.size(); i < size; ++i) {
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
////////////////////////////////////////////////////////////////////////////////
AnimatedSprite::AnimatedSprite() :
    mFlags(Flag::NONE)
,   mAccumTime(0.f)
,   mAnimTime(0.f)
,   mTimeFactor(0.f)
,   mAnimIndex(0u)
{
}

AnimatedSprite::~AnimatedSprite()
{
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
    const unsigned int frameIndex = getIndexFromTime(mAccumTime,
                                                     mTimeFactor,
                                                     mAnimations[mAnimIndex].begin,
                                                     mAnimations[mAnimIndex].end);
    if (frameIndex != mFrameIndex){
        // configure the new frame
        mFrameIndex = frameIndex;
        configureRect(frameIndex);
    }

}


} /* namespace utils */
