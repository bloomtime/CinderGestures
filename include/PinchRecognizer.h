#pragma once

#include "cinder/Matrix.h"

#include "PhasedGestureRecognizer.h"
#include "PinchEvent.h"


namespace cinder { namespace app {


class PinchRecognizer : public PhasedGestureRecognizer<PinchEvent> {
protected:
    
    Vec2f mTouch1Start, mTouch2Start;
    Vec2f mTouch1Prev,  mTouch2Prev;
    
    PinchEvent mLastDispatchedEvent;
    
    // are we pinching yet?
    bool mIsPinching;
    
    // handle basic touch events from mApp...
    bool touchesBegan(TouchEvent event);
    bool touchesMoved(TouchEvent event);
    bool touchesEnded(TouchEvent event);

public:
    
    PinchRecognizer() : PhasedGestureRecognizer<PinchEvent>(), mIsPinching(false) {}
    
    void init(AppType *app)
    {
        PhasedGestureRecognizer<PinchEvent>::init(app);
        mIsPinching = false;
    }

};

    
bool PinchRecognizer::touchesBegan(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
    if(!mIsPinching && touches.size() == 2)
    {
        mIsPinching = true;
        
        mTouch1Prev = mTouch1Start = touches[0].getPos();
        mTouch2Prev = mTouch2Start = touches[1].getPos();
        
        mLastDispatchedEvent = PinchEvent();
        mCallbacksBegan.call(mLastDispatchedEvent);
        
        return true;
    }
    return false;
}

bool PinchRecognizer::touchesMoved(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
    if(mIsPinching && touches.size() == 2)
    {
        mLastDispatchedEvent = PinchEvent(mTouch1Start, mTouch2Start, mTouch1Prev, mTouch2Prev, touches[0].getPos(), touches[1].getPos());
        mCallbacksMoved.call(mLastDispatchedEvent);
        
        mTouch1Prev = touches[0].getPos();
        mTouch2Prev = touches[1].getPos();
        
        return true;
    }
    return false;
}

bool PinchRecognizer::touchesEnded(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
    if(mIsPinching && touches.size() < 2){
        mCallbacksEnded.call(PinchEvent());
        mIsPinching = false;
        return true;
    }
    return false;
}

    
} } // namespace cinder::app
