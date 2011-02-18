#pragma once

#include "cinder/app/TouchEvent.h"
#include "PhasedGestureRecognizer.h"
#include "PinchEvent.h"


namespace cinder { namespace app {


class PinchRecognizer : public PhasedGestureRecognizer<PinchEvent> {
protected:
    
    // keep track of previous touch point locations by id
    std::map<uint32_t, Vec2f> mStartPositions;
    
    Vec2f mStartOrigin;
    float mStartDist, mStartRot;
    
    PinchEvent mLastDispatchedEvent;
    
    // are we pinching yet?
    bool mIsPinching;
    
    // handle basic touch events from mApp...
    bool touchesBegan(TouchEvent event);
    bool touchesMoved(TouchEvent event);
    bool touchesEnded(TouchEvent event);

public:		
    
    PinchRecognizer() : PhasedGestureRecognizer<PinchEvent>(), mIsPinching(false) {}
    
    void init(AppType *app){
        PhasedGestureRecognizer<PinchEvent>::init(app);
        mStartPositions.clear();
        mIsPinching = false;
    }

};

    
bool PinchRecognizer::touchesBegan(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
    if(!mIsPinching && touches.size() == 2){
        for(int i = 0; i < 2; i++)
            mStartPositions[touches[i].getId()] = touches[i].getPos();
        Vec2f tp1 = touches[0].getPos();
        Vec2f tp2 = touches[1].getPos();
        mStartOrigin = tp1.lerp(0.5f, tp2);
        mStartDist   = tp1.distance(tp2);
        mStartRot    = math<float>::atan2(tp2.y - tp1.y, tp2.x - tp1.x);
        mIsPinching  = true;
        mLastDispatchedEvent = PinchEvent(Vec2f(), mStartOrigin, 0.0f, mStartOrigin, 1.0f);
        mCallbacksBegan.call(mLastDispatchedEvent);
        
        return true;
    }
    return false;
}

bool PinchRecognizer::touchesMoved(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
    if(mIsPinching){
        if(touches.size() == 2){
            Vec2f tp1 = touches[0].getPos();
            Vec2f tp2 = touches[1].getPos();
            
            Vec2f nowOrigin = tp1.lerp(0.5f, tp2);
            float nowDist   = tp1.distance(tp2);
            float nowRot    = atan2(tp2.y - tp1.y, tp2.x - tp1.x);
            
            Vec2f translation = nowOrigin - mStartOrigin;
            float scale       = nowDist / mStartDist;
            float rotation    = nowRot - mStartRot;
            
            mLastDispatchedEvent = PinchEvent(translation, nowOrigin, rotation, mStartOrigin, scale);
            mCallbacksMoved.call(mLastDispatchedEvent);
            
            return true;
        }
    }
    return false;
}

bool PinchRecognizer::touchesEnded(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
    if(mIsPinching && touches.size() < 2){
        mCallbacksEnded.call(mLastDispatchedEvent);
        mIsPinching = false;
        return true;
    }
    return false;
}

    
} } // namespace cinder::app
