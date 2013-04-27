/*
 * TextureAtlas.h
 *
 *  Created on: Apr 27, 2013
 *      Author: agustin
 */

#ifndef TEXTUREATLAS_H_
#define TEXTUREATLAS_H_

#include <SFML/Graphics/Rect.hpp>

#include <common/debug/DebugUtil.h>
#include <common/Forwards.h>

namespace utils {

class TextureAtlas
{
public:
    /**
     * @brief Configure the Texture atlas with a texture and the sizes
     * @param texture   The texture to be used (we get the size from here)
     * @param nColumns  The number of columns that the texture has (atlas)
     * @param nRows     The number of rows (of the atlas)
     * @note For now we will only support grid atlas
     */
    void
    configure(TexturePtr &texture, unsigned int nColumns, unsigned int nRows);

    /**
     * @brief Get the rectangle associated to the configured texture given the
     *        index of the atlas we want. The indices are given in this order:
     *        0   |   1   |   2   |   3
     *        4   |   5   |   6   |   7
     *        ..........................
     *        N-3 |  N-2  |  N-1  |   N
     */
    inline void
    getRect(unsigned int index, sf::IntRect &rect) const;
    inline void
    getRect(unsigned int col, unsigned int row, sf::IntRect &rect) const;

    /**
     * @brief Query methods
     */
    inline unsigned int
    numRows(void) const;
    inline unsigned int
    numColumns(void) const;
    inline const sf::IntRect&
    rect(void) const;

private:
    sf::IntRect mRect;
    unsigned int mNumRows;
    unsigned int mNumColumns;
};



// Inline implementations
//

inline void
TextureAtlas::getRect(unsigned int index, sf::IntRect &rect) const
{
    const unsigned int row = index / mNumColumns;
    const unsigned int col = index - (mNumColumns * row);

    ASSERT(row < mNumRows);
    ASSERT(col < mNumColumns);

    // put the rectangle over there
    rect.top = row * mRect.height;
    rect.left = col * mRect.width;
    rect.height = mRect.height;
    rect.width = mRect.width;
}

inline void
TextureAtlas::getRect(unsigned int col, unsigned int row, sf::IntRect &rect) const
{
    ASSERT(row < mNumRows);
    ASSERT(col < mNumColumns);

    const unsigned int index = row * mNumColumns + col;
    getRect(index, rect);
}

inline unsigned int
TextureAtlas::numRows(void) const
{
    return mNumRows;
}

inline unsigned int
TextureAtlas::numColumns(void) const
{
    return mNumColumns;
}

inline const sf::IntRect&
TextureAtlas::rect(void) const
{
    return mRect;
}

} /* namespace utils */
#endif /* TEXTUREATLAS_H_ */
