/*
 * AnimatedSprite.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: agustin
 */

#include "AnimatedSprite.h"

#include <SFML/System/Vector2.hpp>

#include <debug/DebugUtil.h>

namespace ui {

////////////////////////////////////////////////////////////////////////////////
bool
AnimatedSprite::checkAnimTable(const std::vector<AnimIndices> &animation) const
{
    // this function is ultra verbose just to print almost all the possible
    // errors at once
    bool error = false;
    const std::size_t numSprites = mNumColumns * mNumRows;
    for(std::size_t i = 0, size = animation.size(); i < size; ++i){
        AnimIndices &animI = animation[i];
        if (animI.animTime < 0.f) {
            error = true;
            debugERROR("Animation %u has invalid time\n", i);
        }
        if (animI.begin > numSprites) {
            error = true;
            debugERROR("Animation %u has invalid begin: %u\n", i, animI.begin);
        }
        if (animI.end > numSprites) {
            error = true;
            debugERROR("Animation %u has invalid end: %u\n", i, animI.end);
        }
        if (animI.begin > animI.end) {
            error = true;
            debugERROR("Animation %u has invalid begin[%u] or end[%u]\n",
                animI.begin, animI.end);
        }
    }
    return error;
}

////////////////////////////////////////////////////////////////////////////////
void
AnimatedSprite::configureRect(const std::size_t index)
{
    const std::size_t row = index / mNumColumns;
    const std::size_t col = index - (mNumColumns * row);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
AnimatedSprite::AnimatedSprite() :
    mFlags(Flag::NONE)
,   mAccumTime(0.f)
,   mAnimTime(0.f)
,   mNumColumns(0u)
,   mNumRows(0u)
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

    // texture loaded ok, configure the rectangle size now
    sf::Vector2 textSize = mTexture->getSize();
    mRect.width = textSize.x / numColumns;
    mRect.height = textSize.y / numRows;

    mNumColumns = numColumns;
    mNumRows = numRows;
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
    mAnimTime = (time < 0.f) ? mAnimations[animID].time : time;
    mAccumTime = 0.f;

    // configure the rectangle
    mAnimIndex = mAnimations[animID].begin;

}

////////////////////////////////////////////////////////////////////////////////
void
AnimatedSprite::update(float timeFrame)
{

}


} /* namespace ui */
