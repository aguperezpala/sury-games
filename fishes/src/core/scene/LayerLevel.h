/*
 * LayerLevel.h
 *
 *  Created on: Apr 22, 2013
 *      Author: agustin
 */

#ifndef LAYERLEVEL_H_
#define LAYERLEVEL_H_


namespace scene {

// define the different LayerLevel's that we will handle, this way we can
// do a faster drawing
enum LayerLevel {
    LAYER_0 = 0,    // the things that will be rendered on the back
    LAYER_1,        // things that will be rendered on the middle
    LAYER_2,        // on the front of all?

    SIZE,   // DO NOT MODIFY THIS
};

}


#endif /* LAYERLEVEL_H_ */
