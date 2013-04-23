/*
 * Forwards.h
 *
 *  Created on: Apr 21, 2013
 *      Author: agustin
 */

#ifndef FORWARDS_H_
#define FORWARDS_H_

#include <memory>


namespace sf {
class Texture;
}

// Forward declaration and typedefs
//

// Resources typedefs
typedef std::shared_ptr<sf::Texture> TexturePtr;

#endif /* FORWARDS_H_ */
