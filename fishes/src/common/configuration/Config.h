/*
 * Config.h
 *
 *  Created on: 06/05/2013
 *      Author: Harriague
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <SFML/Graphics/Rect.hpp>

#include <core/math/AABB.h>
#include <common/debug/DebugUtil.h>


#define ASSERT_RANGE01(x) ASSERT(x >= 0.f);ASSERT(x <= 1.f);

namespace common {

class Config
{
public:
    /**
     * @brief Returns the instance of the singleton
     */
    static Config& instance(void)
    {
        static Config instance;
        return instance;
    }
    ~Config();

    /**
     * @brief Set the size of the current window
     * @param width     The width of the window
     * @param height    The height of the param
     */
    inline void
    setWindowSize(unsigned int width, unsigned int height);


    ////////////////////////////////////////////////////////////////////////////
    //                          Resizing methods                              //
    ////////////////////////////////////////////////////////////////////////////
    //


    /**
     * @brief Helper methods to resize different kind of objects. We always
     *        will use the sizes [0,1] to resize the element as corresponds
     * @param width     The value between [0,1] to be used as reference value
     * @param height    The value between [0,1] to be used as reference value
     */
    inline sf::IntRect
    resize(const sf::FloatRect& rect) const;
    inline math::AABBui
    resize(const math::AABBf& rect) const;

    // vector resize
    //
    inline sf::Vector2u
    resize(const sf::Vector2f& v);
    inline math::Vector2ui
    resize(const math::Vector2f& v);

    // simple value resize
    //
    inline unsigned int
    resizeWidth(float refValue) const;
    inline unsigned int
    resizeHeight(float refValue) const;


private:
    Config();
    Config(const Config&);
    Config& operator=(const Config&);

private:
    unsigned int mWinWidth;
    unsigned int mWinHeight;
};






// Inline impl
//

inline void
Config::setWindowSize(unsigned int width, unsigned int height)
{
    mWinWidth = width;
    mWinHeight = height;
}

inline sf::IntRect
Config::resize(const sf::FloatRect& rect) const
{
    ASSERT_RANGE01(rect.width);
    ASSERT_RANGE01(rect.height);
    ASSERT_RANGE01(rect.left);
    ASSERT_RANGE01(rect.top);
    return sf::IntRect(rect.left * mWinWidth, rect.top * mWinHeight,
                       rect.width * mWinWidth, rect.height * mWinHeight);
}
inline math::AABBui
Config::resize(const math::AABBf& rect) const
{
    ASSERT_RANGE01(rect.br.x);
    ASSERT_RANGE01(rect.br.y);
    ASSERT_RANGE01(rect.tl.x);
    ASSERT_RANGE01(rect.tl.y);

    return math::AABBui(rect.tl.x * mWinWidth, rect.tl.y * mWinHeight,
                        rect.br.x * mWinWidth, rect.br.y * mWinHeight);
}

// vector resize
//
inline sf::Vector2u
Config::resize(const sf::Vector2f& v)
{
    ASSERT_RANGE01(v.x);
    ASSERT_RANGE01(v.y);
    return sf::Vector2u(v.x * mWinWidth, v.y * mWinHeight);
}
inline math::Vector2ui
Config::resize(const math::Vector2f& v)
{
    ASSERT_RANGE01(v.x);
    ASSERT_RANGE01(v.y);

    return math::Vector2ui(v.x * mWinWidth, v.y * mWinHeight);
}

// simple value resize
//
inline unsigned int
Config::resizeWidth(float refValue) const
{
    ASSERT_RANGE01(refValue);
    return refValue * mWinWidth;
}
inline unsigned int
Config::resizeHeight(float refValue) const
{
    ASSERT_RANGE01(refValue);
    return refValue * mWinHeight;
}

} /* namespace common */
#endif /* CONFIG_H_ */
