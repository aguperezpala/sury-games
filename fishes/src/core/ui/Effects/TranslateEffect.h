/*
 * TranslateEffect.h
 *
 *  Created on: 07/05/2013
 *      Author: Harriague
 */

#ifndef TRANSLATEEFFECT_H_
#define TRANSLATEEFFECT_H_


#include <core/math/Vector2.h>
#include <common/global/GlobalData.h>

#include "../EffectsDefs.h"

namespace ui {

template<typename FunctionType>
class TranslateEffect : public Effect
{
public:
    TranslateEffect(Element* element) : Effect(element) {};
    virtual
    ~TranslateEffect() {};

    /**
     * @brief Configure the effect, this will use the FunctionType interface
     *        so we need to check if FunctionType has configure method
     */
    inline void
    configure(const math::Vector2f& begPos,
              const math::Vector2f& endPos,
              float time);

    /**
     * @brief We need to implement this method if we want to initialize the
     *        effect
     */
    virtual inline void
    starting(void);

    /**
     * @brief Method called every frame to update the logic of this element
     */
    virtual void
    update(void) = 0;

private:
    FunctionType mFun;
    float mTime;
    math::Vector2f mBegPos;
    math::Vector2f mEndPos;
};




// Inline Impl
//
template<typename FunctionType>
inline void
TranslateEffect<FunctionType>::configure(const math::Vector2f& begPos,
                                         const math::Vector2f& endPos,
                                         float time)
{
    mBegPos = begPos;
    mEndPos = endPos;
    mTime = time;
}

template<typename FunctionType>
inline void
TranslateEffect<FunctionType>::starting(void)
{
    mFun.configure(mBegPos, mEndPos, mTime);
    dettachElement();
}

template<typename FunctionType>
inline void
TranslateEffect<FunctionType>::update(void)
{
    ASSERT(mElement);

    // update the position of the element
    mElement->setPosition(mFun(common::GlobalData::LAST_TIME_FRAME));
    if (mFun.finish()) {
        setFinish(true);
    }
}

// We will do some typedefs here of the typical translate functions


} /* namespace ui */
#endif /* TRANSLATEEFFECT_H_ */
