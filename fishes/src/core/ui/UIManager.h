/*
 * UIManager.h
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#ifndef UIMANAGER_H_
#define UIMANAGER_H_

namespace ui {

class UIManager {
public:
    UIManager();
    ~UIManager();

    /**
     * @brief Builds the MenuMatrix
     * @note The coordinates system of the window are tl = (0,0) and
     *       br = (windowWidth, windowHeight).
     *
     * @param ww    The windows Width
     * @param wh    The windows hegith
     * @param cnX   The number of cells used in X
     * @param cnY   The number of cells used in Y
     */
    bool
    build(unsigned int ww, unsigned int wh, unsigned int cnX, unsigned int cnY);

    /**
     * Add/Remove/Check a new IMenu to the system
     */
    void addMenu(IMenu *menu);
    void removeMenu(IMenu *menu);
    bool hasMenu(IMenu *menu);

    /**
     * Update the menu
     * @return  This function will return true if the mouse is over some object
     *          or false if we are not over any object
     * @note    This class use the GlobalObjects::mouse instance
     */
    bool update(void);
};

}

#endif /* UIMANAGER_H_ */
