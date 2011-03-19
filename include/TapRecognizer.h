#pragma once

#include "cinder/Thread.h"
#include "cinder/Function.h"
#include "GestureRecognizer.h"
#include "DoubleTapEvent.h"
#include "SingleTapEvent.h"

#include <boost/date_time.hpp>
#include <boost/bind.hpp>


namespace cinder {

    
class TapRecognizer : public GestureRecognizer {
protected:
    
    // keep track of the touches from the last touch ended event
    uint32_t mLastTouchId;
    Vec2f    mLastTouchPos;
    double   mLastTouchBeganTime, mLastTouchEndedTime;
    bool     mLastTouchWaiting;
    
    float  mDoubleTapMaxDistance, mSingleTapMaxDistance;
    double mDoubleTapMaxWait, mSingleTapMaxWait;
    
    std::mutex mTimeoutMutex;
    void singleTapTimeout(){
        ThreadSetup setup();
        boost::this_thread::sleep(boost::posix_time::milliseconds(mDoubleTapMaxWait * 1000));
        mTimeoutMutex.lock();
        if(mLastTouchWaiting){
            mLastTouchWaiting = false;
            mCallbacksSingleTap.call(SingleTapEvent(mLastTouchPos, mLastTouchEndedTime));
        }
        mTimeoutMutex.unlock();
    }
    
    // override touch end handling from GestureAnalyzer base...
    bool touchesEnded(TouchEvent event);
    bool touchesBegan(TouchEvent event);
    bool touchesMoved(TouchEvent event) { return false; } // TODO: should they be optional not required?
    
    // keep track of listeners for our own events...
    CallbackMgr<bool(DoubleTapEvent)> mCallbacksDoubleTap;
    CallbackMgr<bool(SingleTapEvent)> mCallbacksSingleTap;

public:		
    
    TapRecognizer() : GestureRecognizer(), mDoubleTapMaxDistance(25.0f), mSingleTapMaxDistance(25.0f), mSingleTapMaxWait(0.2), mDoubleTapMaxWait(0.2) {}
    
    void init(AppType *app) {
        GestureRecognizer::init(app);
        mLastTouchWaiting = false;
    }
    
    /* 
     * these registerXXX methods are convenience functions 
     * so that you can type:
     *
     *     mTapRecognizer.registerDoubleTap(this, &MyApp::doubleTap);
     *
     * rather than:
     *
     *     mTapRecognizer.mCallbacksDoubleTap.registerCb(std::bind1st(std::mem_fun(&MyApp::doubleTap), this));
     *     
     * where doubleTap is a function in your app:
     *
     *     bool MyApp::doubleTap(DoubleTapEvent event) { ... }
     *
     */
    template<typename T>
    CallbackId registerDoubleTap( T *obj, bool (T::*callback)(DoubleTapEvent) ){
        return mCallbacksDoubleTap.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }

    template<typename T>
    CallbackId registerSingleTap( T *obj, bool (T::*callback)(SingleTapEvent) ){
        return mCallbacksSingleTap.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }
    
};

bool TapRecognizer::touchesBegan(TouchEvent event)
{
    mLastTouchId        = event.getTouches().back().getId();
    mLastTouchPos       = event.getTouches().back().getPos();
    mLastTouchBeganTime = event.getTouches().back().getTime();
    return false;
}

bool TapRecognizer::touchesEnded(TouchEvent event)
{
    std::vector<TouchEvent::Touch> touches = event.getTouches();
    
    mTimeoutMutex.lock();
    
    bool startTimeout = false;
    bool doubleTapFired = false;
    if(mLastTouchWaiting){
        for(std::vector<TouchEvent::Touch>::iterator it = touches.begin(); it != touches.end(); ++it){
            if(mLastTouchPos.distance(it->getPos()) <= mDoubleTapMaxDistance){
                mLastTouchWaiting = false;
                doubleTapFired = true;
                mCallbacksDoubleTap.call(DoubleTapEvent(mLastTouchPos, it->getTime()));
                break;
            }
        }
    }
    else{
        for(std::vector<TouchEvent::Touch>::iterator it = touches.begin(); it != touches.end(); ++it){
            if(it->getId() == mLastTouchId &&
               it->getTime() - mLastTouchBeganTime <= mSingleTapMaxWait &&
               it->getPos().distance(mLastTouchPos) <= mSingleTapMaxDistance){
                mLastTouchPos       = it->getPos();
                mLastTouchEndedTime = it->getTime();
                mLastTouchWaiting = true;
                startTimeout = true;
                break;
            }
        }
    }
    
    mTimeoutMutex.unlock();
    
    if(startTimeout){
        std::thread timeoutThread(boost::bind(&TapRecognizer::singleTapTimeout, this));
        return true;
    }
    
    return doubleTapFired;
}


} // namespace cinder
