#pragma once

#include "cinder/Matrix.h"

#include "PhasedGestureRecognizer.h"
#include "PinchEvent.h"

#include <vector>

namespace cinder {


class PinchRecognizer : public PhasedGestureRecognizer<PinchEvent> {
protected:

    std::pair<PinchEvent::Touch, PinchEvent::Touch> mTouchPair;
    
    TouchEvent::Touch *mLastTouch;
    
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
    
    void rejectPinch()
    {
        mIsPinching = false;
    }

};


bool PinchRecognizer::touchesBegan(TouchEvent event)
{
    const std::vector<TouchEvent::Touch> &touches = event.getTouches();
    if(!mIsPinching && touches.size() == 2){
        mIsPinching = true;
        
        PinchEvent::Touch t1 = { touches[0].getId(), touches[0].getPos(), touches[0].getPos(), touches[0].getPos() };
        PinchEvent::Touch t2 = { touches[1].getId(), touches[1].getPos(), touches[1].getPos(), touches[1].getPos() };
        
        mTouchPair = t1.mId < t2.mId ? std::make_pair(t1, t2) : std::make_pair(t2, t1);
        
        mCallbacksBegan.call(PinchEvent(touches));
    }
    return false;
}

bool PinchRecognizer::touchesMoved(TouchEvent event)
{
    if(mIsPinching){
        bool shouldFireEvent = false;
        const vector<TouchEvent::Touch> &touches = event.getTouches();
        for(vector<TouchEvent::Touch>::const_iterator it = touches.begin(); it != touches.end(); ++it){
            if(it->getId() == mTouchPair.first.mId){
                mTouchPair.first.mPos = it->getPos();
                shouldFireEvent = true;
            }
            else if(it->getId() == mTouchPair.second.mId){
                mTouchPair.second.mPos = it->getPos();
                shouldFireEvent = true;
            }
        }
        if(shouldFireEvent){
            mCallbacksMoved.call(PinchEvent(touches, mTouchPair, mApp->getWindowSize()));
            mTouchPair.first.mPosPrev  = mTouchPair.first.mPos;
            mTouchPair.second.mPosPrev = mTouchPair.second.mPos;
        }
    }
    return false;
}

bool PinchRecognizer::touchesEnded(TouchEvent event)
{
    if(mIsPinching){
        for(vector<TouchEvent::Touch>::const_iterator it = event.getTouches().begin(); it != event.getTouches().end(); ++it){
            if(it->getId() == mTouchPair.first.mId || it->getId() == mTouchPair.second.mId){
                mCallbacksEnded.call(PinchEvent(event.getTouches()));
                mIsPinching = false;
                break;
            }
        }
    }
    return false;
}

    
} // namespace cinder
