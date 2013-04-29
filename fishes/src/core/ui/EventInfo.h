/*
 * EventInfo.h
 *
 *  Created on: Apr 28, 2013
 *      Author: agustin
 */

#ifndef EVENTINFO_H_
#define EVENTINFO_H_

#include <cstring>

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
        IOInfo() { clear(); }

        unsigned char LeftMousePressed : 1;
        unsigned char RightMousePressed : 1;
        unsigned char MiddleMousePressed : 1;

        // check for mouse info
        //
        bool someMouseButtonPressed(void) const
        {
            return LeftMousePressed || RightMousePressed || MiddleMousePressed;
        }

        // clear all
        //
        void clear(void)
        {
            std::memset(this, 0, sizeof(IOInfo));
        }

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
