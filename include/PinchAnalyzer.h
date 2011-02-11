/*
 *  PinchAnalyzer.h
 *  CinderGestures
 *
 *  Created by Tom Carden on 2/8/11.
 *  Copyright 2011 Bloom Studio Inc. All rights reserved.
 */

#pragma once

#include "cinder/app/TouchEvent.h"
#include "cinder/Matrix.h"
#include "GestureAnalyzer.h"
#include "PinchEvent.h"

namespace cinder { namespace app {

class PinchAnalyzer : public GestureAnalyzer {
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
    
    // keep track of listeners for our own events...
    CallbackMgr<bool(PinchEvent)> mCallbacksPinchBegan;
    CallbackMgr<bool(PinchEvent)> mCallbacksPinchMoved;
    CallbackMgr<bool(PinchEvent)> mCallbacksPinchEnded;

public:		
    
    PinchAnalyzer() : GestureAnalyzer(), mIsPinching(false) {}
    
    void init(AppBasic *app){
        GestureAnalyzer::init(app);
        mStartPositions.clear();
        mIsPinching = false;
    }
    
    template<typename T>
    CallbackId registerPinchBegan(T *obj, bool (T::*callback)(PinchEvent)){
        return mCallbacksPinchBegan.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }
    template<typename T>
    CallbackId registerPinchMoved(T *obj, bool (T::*callback)(PinchEvent)){
        return mCallbacksPinchMoved.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }
    template<typename T>
    CallbackId registerPinchEnded(T *obj, bool (T::*callback)(PinchEvent)){
        return mCallbacksPinchEnded.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }

};

    
bool PinchAnalyzer::touchesBegan(TouchEvent event)
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
        mLastDispatchedEvent = PinchEvent(mStartOrigin, Vec2f(), 0.0f, 1.0f);
        mCallbacksPinchBegan.call(mLastDispatchedEvent);
    }
    return true;
}

bool PinchAnalyzer::touchesMoved(TouchEvent event)
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
            
            mLastDispatchedEvent = PinchEvent(nowOrigin, translation, rotation, scale);
            
            mCallbacksPinchMoved.call(mLastDispatchedEvent);
        }
    }
    return true;
}

bool PinchAnalyzer::touchesEnded(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = mApp->getActiveTouches();
    if(mIsPinching && touches.size() < 2) {
        mCallbacksPinchEnded.call(mLastDispatchedEvent);
        mIsPinching = false;
    }
    return true;
}
	
} }
