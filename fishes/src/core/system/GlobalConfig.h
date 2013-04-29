/*
 * GlobalConfig.h
 *
 *  Created on: Apr 22, 2013
 *      Author: agustin
 */

#ifndef GLOBALCONFIG_H_
#define GLOBALCONFIG_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <common/debug/DebugUtil.h>


// Forward
//
namespace utils {
class MouseCursor;
}

namespace sys {

class GlobalConfig
{
public:
    GlobalConfig();
    ~GlobalConfig();

    //
    //      Window associated methods
    //

    /**
     * @brief Render window get / set
     */
    inline const sf::RenderWindow&
    renderWindow(void) const;
    inline sf::RenderWindow&
    renderWindow(void);

    inline void
    setRenderWindow(sf::RenderWindow &rw);

    /**
     * @brief Set/Get some useful global values
     */
    inline const sf::Vector2u&
    windowSize(void) const;
    inline unsigned int
    windowWidth(void) const;
    inline unsigned int
    windowHeight(void) const;

    /**
     * @brief Set/Get MouseCursor instance
     */
    inline utils::MouseCursor&
    mouseCursor(void);
    inline void
    setMouseCursor(utils::MouseCursor &mouseCursor);

private:
    sf::Vector2u mWindowSize;
    sf::RenderWindow *mRenderWindow;
    utils::MouseCursor *mMouseCursor;
};


// Inline impl
//

inline const sf::Vector2u&
GlobalConfig::windowSize(void) const
{
    return mWindowSize;
}
inline unsigned int
GlobalConfig::windowWidth(void) const
{
    return mWindowSize.x;
}
inline unsigned int
GlobalConfig::windowHeight(void) const
{
    return mWindowSize.y;
}

inline const sf::RenderWindow&
GlobalConfig::renderWindow(void) const
{
    ASSERT(mRenderWindow);
    return *mRenderWindow;
}
inline sf::RenderWindow&
GlobalConfig::renderWindow(void)
{
    ASSERT(mRenderWindow);
    return *mRenderWindow;
}
inline void
GlobalConfig::setRenderWindow(sf::RenderWindow &rw)
{
    mRenderWindow = &rw;
    mWindowSize = rw.getSize();
}

inline utils::MouseCursor&
GlobalConfig::mouseCursor(void)
{
    return *mMouseCursor;
}
inline void
GlobalConfig::setMouseCursor(utils::MouseCursor &mouseCursor)
{
    mMouseCursor = &mouseCursor;
}

} /* namespace system */
#endif /* GLOBALCONFIG_H_ */
