/*
 * UpdatableManager.h
 *
 *  Created on: 06/05/2013
 *      Author: Agustin
 */

#ifndef UPDATABLEMANAGER_H_
#define UPDATABLEMANAGER_H_

#include <common/updaters/SingleUpdater.h>

namespace utils {

typedef common::SingleUpdaterElement UpdatableObject;
typedef common::SingleUpdater<UpdatableObject> UpdatableManager;


} /* namespace utils */
#endif /* UPDATABLEMANAGER_H_ */
