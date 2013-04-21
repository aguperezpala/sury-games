/*
 * Resource.h
 *
 *  Created on: Apr 21, 2013
 *      Author: agustin
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <atomic>

namespace resources {

typedef unsigned int ResourceID;

class Resource {
public:
    Resource();
    virtual ~Resource();

    /**
     * @brief Returns the ID associated to this resource
     * @returns The id of this resource
     */
    inline ResourceID
    id(void) const;

private:
    ResourceID mID;

    // atomic id counter
    static std::atomic<ResourceID> sIDCounter;
};


// Inline implementations
//

inline ResourceID
Resource::id(void) const
{
    return mID;
}

}

#endif /* RESOURCE_H_ */
