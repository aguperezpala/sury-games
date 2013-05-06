/*
 * UpdatableObject.h
 *
 *  Created on: 06/05/2013
 *      Author: Agustin
 */

#ifndef UPDATABLEOBJECT_H_
#define UPDATABLEOBJECT_H_

namespace utils {

// Forward
//
class UpdatableManager;


class UpdatableObject {
public:
	UpdatableObject() {};
	virtual ~UpdatableObject(){};

	/**
	 * @brief Method called every frame to update the logic of this element
	 */
	virtual void
	update(void) = 0;

private:
	friend class UpdatableManager;
	unsigned int id;
};

} /* namespace utils */
#endif /* UPDATABLEOBJECT_H_ */
