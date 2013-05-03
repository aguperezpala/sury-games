/*
 * EventInfo.h
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#ifndef EVENTINFO_H_
#define EVENTINFO_H_

#include <cstring>
#include <bitset>

namespace ui {

struct EventInfo {

    // List all the events we will handle for now
    //
    enum Event {
        None = 0,
        MouseButton,        // the mouse is inside of the element and some button
                            // was pressed.
        MouseInside,        // when the mouse is getting inside of the element
        MouseMoving,        // when the mouse is inside of the element and it is moving
        MouseOutside,       // the mouse was moved outside of the element
    } event;

    struct IOInfo {
        enum Mouse {
            LeftButton = 0,
            RightButton,
            MiddleButton,

            NumButtons,
        };
        IOInfo() { clear(); }

        // check for mouse info
        //
        bool someMouseButtonPressed(void) const
        {
            return mouseButtons.any();
        }

        // check if some button has released
        //
        bool someMouseButtonReleased(void) const
        {
            return mouseButtonsLast.any();
        }

        // Update the state of the buttons of the mouse
        //
        void updateState(Mouse button, bool pressed)
        {
            mouseButtonsLast[button] = mouseButtons[button];
            mouseButtons.set(button, pressed);
        }

        // check for a particular button
        //
        bool checkButtonPressed(Mouse button) const
        {
            return mouseButtons[button];
        }

        // check if some button was released
        //
        bool checkButtonReleased(Mouse button) const
        {
            return !mouseButtons[button] && mouseButtonsLast[button];
        }

        // clear all
        //
        void clear(void)
        {
            mouseButtons.reset();
            mouseButtonsLast.reset();
        }

    private:
        std::bitset<NumButtons> mouseButtons;
        std::bitset<NumButtons> mouseButtonsLast;

    } ioInfo;



    // clear all the EventInfo
    //
    void clear()
    {
        event = Event::None;
        ioInfo.clear();
    }

};

}

#endif /* EVENTINFO_H_ */
