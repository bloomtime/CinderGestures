#pragma once

#include "cinder/Cinder.h"
#if defined (CINDER_COCOA_TOUCH)
#include "cinder/app/AppCocoaTouch.h"
#else
#include "cinder/app/AppBasic.h"
#endif

using namespace ci::app;

namespace cinder {

    
class GestureRecognizer {
protected:

#if defined (CINDER_COCOA_TOUCH)
    typedef AppCocoaTouch AppType;
#else
    typedef AppBasic AppType;
#endif
    
    // your Cinder App (usually passed in as 'this' in setup)
    AppType *mApp;
    
    // subclasses must handle basic touch events from mApp...
    virtual bool touchesBegan(TouchEvent event) = 0;
    virtual bool touchesMoved(TouchEvent event) = 0;
    virtual bool touchesEnded(TouchEvent event) = 0;
    
    // for cleanup in destructor/setup, keep callback ids
    CallbackId mTouchesBeganCb;
    CallbackId mTouchesMovedCb;
    CallbackId mTouchesEndedCb;
    
    std::function<bool(TouchEvent::Touch)> mKeepTouchCb;
    
    bool privateTouchesBegan(TouchEvent event){
        // Allow the Recognizer to Keep/Reject touches.
        // If no touches are kept then the event is not fired and we defer to the next listener.
        if(mKeepTouchCb){
            std::vector<TouchEvent::Touch> touchList;
            for(std::vector<TouchEvent::Touch>::const_iterator it = event.getTouches().begin(); it != event.getTouches().end(); ++it){
                if(mKeepTouchCb(*it))
                    touchList.push_back(*it);
            }
            if(touchList.size() > 0)
                return touchesBegan(TouchEvent(touchList));
            return false;
        }
        return touchesBegan(event);
    }
    
    void registerTouchCallbacks(){
        if(mApp){
            mTouchesBeganCb = mApp->registerTouchesBegan(this, &GestureRecognizer::privateTouchesBegan);
            mTouchesMovedCb = mApp->registerTouchesMoved(this, &GestureRecognizer::touchesMoved);
            mTouchesEndedCb = mApp->registerTouchesEnded(this, &GestureRecognizer::touchesEnded);
        }
    }
    void unregisterTouchCallbacks(){
        if(mApp){
            mApp->unregisterTouchesBegan(mTouchesBeganCb);
            mApp->unregisterTouchesMoved(mTouchesMovedCb);
            mApp->unregisterTouchesEnded(mTouchesEndedCb);
        }
    }
            
public:		

    GestureRecognizer() : mApp(0) {}

    ~GestureRecognizer(){
        unregisterTouchCallbacks();
    }
    
    virtual void init(AppType *app)
    {
        unregisterTouchCallbacks();
        mApp = app;
        registerTouchCallbacks();
    }
    
    template<typename T>
    void setKeepTouchCallback(T *obj, bool(T::*callback)(TouchEvent::Touch)){
        mKeepTouchCb = std::bind1st(std::mem_fun(callback), obj);
    }

};
	
    
} // namespace cinder::app
