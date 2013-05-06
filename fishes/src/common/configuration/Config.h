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
    inline void
    resize(sf::FloatRect& rect) const;
    inline void
    resize(sf::IntRect& rect, float width, float height) const;
    inline void
    resize(math::AABBf& rect) const;
    inline void
    resize(math::AABBui& rect, float width, float height) const;

    template<typename T>
    inline void
    resizeWidth(T& value, float refValue) const;
    template<typename T>
    inline void
    resizeHeight(T& value, float refValue) const;


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
Config::resize(sf::FloatRect& rect) const
{
    ASSERT(rect.width > 0.f && rect.width <= 1.0f);
    ASSERT(rect.height > 0.f && rect.height <= 1.0f);
    rect.width *=  mWinWidth;
    rect.height *= mWinHeight;
}
inline void
Config::resize(sf::IntRect& rect, float width, float height) const
{
    ASSERT(width > 0.f && width <= 1.0f);
    ASSERT(height > 0.f && height <= 1.0f);

    rect.width = width * mWinWidth;
    rect.height = height * mWinHeight;
}

inline void
Config::resize(math::AABBf& rect) const
{
    const float width = rect.getWidth();
    const float height = rect.getHeight();

    ASSERT(width > 0.f && width <= 1.0f);
    ASSERT(height > 0.f && height <= 1.0f);

    rect.setSize(width * mWinWidth, height * mWinHeight);
}
inline void
Config::resize(math::AABBui& rect, float width, float height) const
{
    ASSERT(width > 0.f && width <= 1.0f);
    ASSERT(height > 0.f && height <= 1.0f);

    rect.setSize(width * mWinWidth, height * mWinHeight);
}

template<typename T>
inline void
Config::resizeWidth<T>(T& value, float refValue) const
{
    ASSERT(refValue > 0.f && refValue <= 1.0f);
    value = mWinWidth * refValue;
}
template<typename T>
inline void
Config::resizeHeight<T>(T& value, float refValue) const
{
    ASSERT(refValue > 0.f && refValue <= 1.0f);
    value = mWinHeight * refValue;
}

} /* namespace common */
#endif /* CONFIG_H_ */
