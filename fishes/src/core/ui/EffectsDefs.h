/*
 * EffectsDefs.h
 *
 *  Created on: 07/05/2013
 *      Author: Harriague
 */

#ifndef EFFECTSDEFS_H_
#define EFFECTSDEFS_H_

#include <common/updaters/SingleUpdater.h>

#include <boost/signal.hpp>
#include <boost/signals/slot.hpp>

#include "Element.h"

namespace ui {

// Define the manager and the effect element interfaces
//
class Effect : public common::SingleUpdaterElement {
public:

    // define the signals types
    //
    typedef boost::signals::scoped_connection Connection;
    typedef boost::signal<void (Effect *)> Signal;

public:
    Effect(Element* element = 0) : mElement(element) {};
    virtual ~Effect() {};

    /**
     * @brief Set/Get the element associated to this effect
     */
    Element*
    element(void)
    {
        return mElement;
    }
    void
    setElement(Element* e)
    {
        mElement = e;
    }

    /**
     * @brief Add/remove callbacks here
     * @param callback  The subscriber to receive the event for when this effect
     *                  finish
     */
    Connection
    addButtonCallback(const Signal::slot_type& subscriber)
    {
        return mSignal.connect(subscriber);
    }

    /**
     * @brief Method called when the element finish its update.
     */
    virtual void
    finished(void)
    {
        attachElement();
        mSignal(this);
    };

    /**
     * @brief We need to implement this method if we want to initialize the
     *        effect
     */
    virtual void
    starting(void);

protected:

    /**
     * @brief Helper methods to deactivate / activate the associated element
     *        that should be called before / after we start / end the effect
     */
    inline void
    attachElement(void)
    {
        ASSERT(mElement);
        mElement->activate(true);
    }
    inline void
    dettachElement(void)
    {
        ASSERT(mElement);
        mElement->activate(false);
    }

protected:
    Element *mElement;

private:
    Signal mSignal;
};

typedef common::SingleUpdater<Effect> EffectManager;

}


#endif /* EFFECTSDEFS_H_ */
