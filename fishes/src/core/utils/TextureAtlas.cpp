/*
 * TextureAtlas.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: agustin
 */

#include "TextureAtlas.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace utils {


void
TextureAtlas::configure(TexturePtr &texture, unsigned int nColumns, unsigned int nRows)
{
    ASSERT(texture.get());

    mNumColumns = nColumns;
    mNumRows = nRows;

    const sf::Vector2u textSize = texture->getSize();
    mRect.top = mRect.left = 0;
    mRect.width = textSize.x / mNumColumns;
    mRect.height = textSize.y / mNumRows;
}



} /* namespace utils */
